#include <iostream>
#include <memory>

#include <glog/logging.h>
#include <gflags/gflags.h>

#include <CGAL/Projection_traits_xy_3.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>

#include <OGRE/Ogre.h>
#include <OGRE/OgreRectangle2D.h>
#include <OGRE/OgreConfigFile.h>

#include <cameraController.h>
#include <windowedRenderingApp.h>

#include <defaultGeometryProviders.h>
#include <defaultBufferProviders.h>
#include <uniformPlanarGrid.h>
#include <uniformVoxelGrid.h>
#include <uniformVoxelGridGeometryProvider.h>
#include <geometryRenderable.h>
#include <geometryMesh.h>
#include <prefabs.h>
#include <geometryBackedSelectableObject.h>

#include "separableGeometryInducedField.h"
#include "distanceFieldComputers.h"
#include "fieldSampler.h"
#include "levelSetMeshBuilder.h"
#include "triangleMeshGeometryProvider.h"

// Default flag values
DEFINE_bool(write_generated_level_set_mesh, false,
            "Should the mesh created by the level set mesh builder be written "
            "out to file?");

bool initScene(WindowedRenderingApp& app, const std::string& sceneName) {
  std::string windowName = app.getWindowName();
  Ogre::Root* root = Ogre::Root::getSingletonPtr();
  Ogre::SceneManager* sceneManager =
      root->createSceneManager(Ogre::ST_GENERIC, sceneName);
  sceneManager->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));

  Ogre::Camera* mainCamera = sceneManager->createCamera("PrimaryCamera");
  app.getSelectionManager().setCamera(*mainCamera);
  Ogre::Viewport* viewport =
      root->getRenderTarget(windowName)->addViewport(mainCamera);
  viewport->setBackgroundColour(Ogre::ColourValue(0.5, 0.5, 0.5));

  mainCamera->setAspectRatio((float)viewport->getActualWidth() /
                             viewport->getActualHeight());
  mainCamera->setNearClipDistance(0.1);

  CameraController* cameraController =
      new CameraController("mainCamera", mainCamera);
  cameraController->setYawPitchDist(Ogre::Degree(0), Ogre::Degree(15), 10);
}

template <typename T>
using PositionRenderable = GeometryRenderable<PositionOnlyBufferProvider<T>>;

template <typename T>
using RenderBufferProvider = PositionOnlyBufferProvider<T>;

template <typename T>
using Renderable = GeometryRenderable<RenderBufferProvider<T>>;

template <typename T>
using Meshable = GeometryMeshCreator<RenderBufferProvider<T>>;

template <typename T, typename U = DefaultMaterialPolicy<
                          DefaultRenderPolicy<RenderBufferProvider<T>>>>
using MaterialRenderable =
    GeometryRenderable<RenderBufferProvider<T>,
                       DefaultRenderPolicy<RenderBufferProvider<T>>, U>;

// View class that allows for rendering level set of height field
template <class Field>
class LevelSetMeshVisualizer {
  static constexpr Kernel::FT MAX_LEVEL = 20;
  static constexpr Kernel::FT MIN_LEVEL = 1;

 public:
  LevelSetMeshVisualizer(const Field& inducedField, Ogre::SceneNode* parent)
      : m_inducedField(&inducedField) {
    m_levelSetSceneNode = parent->createChildSceneNode();
    setNormalizedLevel(0.1);
  }

  void setNormalizedLevel(Kernel::FT value) {
    m_value = (MAX_LEVEL - MIN_LEVEL) * value + MIN_LEVEL;
    addLevelSetMeshToScene();
  }

  void addLevelSetMeshToScene() {
    // Customize some materials
    using MaterialPolicyFunctor = std::function<std::string(void)>;
    MaterialPolicyFunctor transparentMeshPolicy =
        []() { return "Materials/DefaultTransparentTriangles"; };

    using TMeshRepresentation = typename LevelSetMeshBuilder<>::Representation;
    using TMeshGeometryProvider =
        TriangleMeshGeometryProvider<TMeshRepresentation>;

    std::function<Kernel::FT(const Kernel::Point_3&)> samplingFunction =
        [ this, inducedFieldCRef = std::cref(*m_inducedField) ](
            const Kernel::Point_3& point) {
      return inducedFieldCRef.get()(point) - m_value;
    };

    LevelSetMeshBuilder<> meshBuilder;
    CGAL::Polyhedron_3<Kernel> meshRep;
    meshBuilder.buildMesh(samplingFunction, Kernel::Sphere_3(CGAL::ORIGIN, 100),
                          1, meshRep);
    TMeshGeometryProvider meshGeometryProvider(meshRep);
    auto meshRenderable =
        new MaterialRenderable<TMeshGeometryProvider, MaterialPolicyFunctor>(
            transparentMeshPolicy);
    meshRenderable->setVertexBufferData(
        RenderBufferProvider<TMeshGeometryProvider>(meshGeometryProvider));

    // Remove all children from scene node, and re-populate with new level-set
    m_levelSetSceneNode->removeAndDestroyAllChildren();
    m_levelSetSceneNode->attachObject(meshRenderable);
  }

 private:
  Kernel::FT m_value;
  Ogre::SceneNode* m_levelSetSceneNode;
  const Field* m_inducedField;
};

template <class Field>
class PlanarDistanceFieldVisualizer {
 public:
  PlanarDistanceFieldVisualizer(const Field& inducedField,
                                Ogre::SceneNode* polyloopsSceneNode)
      : m_inducedField(&inducedField),
        m_polyloopsSceneNode(polyloopsSceneNode),
        m_distanceFieldSceneNode(
            polyloopsSceneNode->getParentSceneNode()->createChildSceneNode()) {}

  void addToScene() {
    // Customize some materials. TODO msati3: Add a node listener to the
    // polyloops scene node here.
    Ogre::AxisAlignedBox loopBounds = m_polyloopsSceneNode->_getWorldAABB();
    UniformPlanarGrid planarGrid(Kernel::Plane_3(0, 0, 1, 0), 10, 10, 6, 6);
    typedef CGAL::Projection_traits_xy_3<Kernel> GeometryTraits;
    typedef CGAL::Delaunay_triangulation_2<
        GeometryTraits,
        CGAL::Triangulation_data_structure_2<
            CGAL::Triangulation_vertex_base_with_info_2<CGAL::Color, Kernel>,
            CGAL::Triangulation_face_base_2<Kernel>>> Triangulation;
    Triangulation triangulation(planarGrid.begin(), planarGrid.end());

    using TMeshGeometryProvider = TriangleMeshGeometryProvider<Triangulation>;
    TMeshGeometryProvider meshGeometryProvider(triangulation);
    auto gridMeshRenderable =
        new Meshable<TMeshGeometryProvider>("scalarFieldHeatMap");
    gridMeshRenderable->setVertexBufferData(
        RenderBufferProvider<TMeshGeometryProvider>(meshGeometryProvider));

    Ogre::Root* root = Ogre::Root::getSingletonPtr();
    Ogre::SceneManager* sceneManager = root->getSceneManager("PrimaryScene");
    Ogre::Entity* gridEntity =
        sceneManager->createEntity("heatMap", "scalarFieldHeatMap");
    gridEntity->setMaterialName("Materials/DefaultTriangles");

    std::function<Kernel::FT(const Kernel::Point_2&)> samplingFunction =
        [ this, inducedFieldCRef = std::cref(*m_inducedField) ](
            const Kernel::Point_2& point) {
      return inducedFieldCRef.get()(point) - m_value;
    };

    Triangulation::Finite_vertices_iterator iter =
        triangulation.finite_vertices_begin();
    for (; iter != triangulation.finite_vertices_end(); ++iter) {
      Kernel::FT sample = samplingFunction(iter->point());
      iter->info() = CGAL::Color(sample, sample, sample);
    }

    // Remove all children from scene node, and re-populate with new level-set
    m_distanceFieldSceneNode->removeAndDestroyAllChildren();
    m_distanceFieldSceneNode->attachObject(gridEntity);
    m_distanceFieldSceneNode->showBoundingBox(true);
  }

 private:
  Kernel::FT m_value;
  Ogre::SceneNode* m_polyloopsSceneNode;
  Ogre::SceneNode* m_distanceFieldSceneNode;
  const Field* m_inducedField;
};

int main(int argc, char* argv[]) {
  google::InitGoogleLogging(argv[0]);

  WindowedRenderingApp app("Smoothing");

  Polyloop_3 p;
  buildPolyloopFromObj("data/loop1.obj", p);

  Polyloop_2 p2D;
  buildPolyloopFromObj("data/loop1_2D.obj", p2D);

  if (app.init(1200, 900)) {
    initScene(app, "PrimaryScene");

    Ogre::Root* root = Ogre::Root::getSingletonPtr();
    Ogre::SceneManager* sceneManager = root->getSceneManager("PrimaryScene");

    using LoopGeometryProvider3D = PolyloopGeometryProvider<Polyloop_3>;
    using LoopGeometryProvider2D = PolyloopGeometryProvider<Polyloop_2>;

    LoopGeometryProvider3D loopGeometryProvider(p);
    auto loopMeshable = new Meshable<LoopGeometryProvider3D>("polyloop1");
    loopMeshable->setVertexBufferData(
        RenderBufferProvider<LoopGeometryProvider3D>(loopGeometryProvider));
    Ogre::Entity* loop1Entity =
        sceneManager->createEntity("entityPolyloop1", "polyloop1");
    loop1Entity->setMaterialName("Materials/DefaultLines");

    sceneManager->getRootSceneNode()->createChildSceneNode()->attachObject(
        loop1Entity);
    GeometryBackedSelectableObject<decltype(p)> selectableLoop(loop1Entity);
    app.getSelectionManager().addSelectableObject(&selectableLoop);

    using Field = SeparableGeometryInducedField<Kernel::Point_3,
                                                SquaredDistanceFieldComputer>;
    Field inducedField;
    inducedField.addGeometry(p);
    auto meshVisualizer = new LevelSetMeshVisualizer<Field>(
        inducedField, sceneManager->getRootSceneNode());

    LoopGeometryProvider2D loopGeometryProvider2D(p2D);
    auto loopMeshable2D = new Meshable<LoopGeometryProvider2D>("polyloop2");
    loopMeshable2D->setVertexBufferData(
        RenderBufferProvider<LoopGeometryProvider2D>(loopGeometryProvider2D));
    Ogre::Entity* loop2Entity =
        sceneManager->createEntity("entityPolyloop2", "polyloop2");
    loop2Entity->setMaterialName("Materials/DefaultLines");

    Ogre::SceneNode* signedDistanceFieldNode =
        sceneManager->getRootSceneNode()->createChildSceneNode();
    signedDistanceFieldNode->attachObject(loop2Entity);

    using SignedField =
        SeparableGeometryInducedField<Kernel::Point_2,
                                      SignedDistanceFieldComputer>;
    SignedField inducedSignedField;
    inducedSignedField.addGeometry(p2D);
    auto planarDistanceFieldVisualizer =
        new PlanarDistanceFieldVisualizer<SignedField>(inducedSignedField,
                                                       signedDistanceFieldNode);
    //planarDistanceFieldVisualizer->addToScene();

    /*Ogre::SceneNode* planeNode =
        sceneManager->getRootSceneNode()->createChildSceneNode();
    planeNode->attachObject(getPrefab(Prefab::PLANE));
    planeNode->setScale(10, 10, 10);*/

    Ogre::SceneNode* axesNode =
        sceneManager->getRootSceneNode()->createChildSceneNode();
    axesNode->attachObject(getPrefab(Prefab::AXES));
    axesNode->setScale(1, 1, 1);

    // Add widgets and setup callbacks
    CEGUI::WindowManager& windowManager = CEGUI::WindowManager::getSingleton();
    CEGUI::Window* averagingLayout =
        windowManager.loadLayoutFromFile("Averaging.layout");
    CEGUI::GUIContext& guiContext =
        CEGUI::System::getSingleton().getDefaultGUIContext();
    guiContext.getRootWindow()->addChild(averagingLayout);
    auto levelSetSliderChangeHandler =
        [&meshVisualizer](const CEGUI::EventArgs& eventArgs) {
          meshVisualizer->setNormalizedLevel(
              static_cast<CEGUI::Slider*>(
                  static_cast<const CEGUI::WindowEventArgs*>(&eventArgs)
                      ->window)->getCurrentValue());
        };
    guiContext.getRootWindow()
        ->getChild("LevelSetSlider")
        ->subscribeEvent(CEGUI::Slider::EventThumbTrackEnded,
                         CEGUI::Event::Subscriber(levelSetSliderChangeHandler));

    /*UniformVoxelGrid voxelGrid(30.0, 5);
    using VoxelGeometryProvider =
        UniformVoxelGridGeometryProvider<VoxelGridCubeProvider>;
    PositionRenderable<VoxelGeometryProvider>* renderableVoxelGrid =
        new PositionRenderable<VoxelGeometryProvider>();
    VoxelGeometryProvider voxelGeometryProvider(voxelGrid);
    renderableVoxelGrid->setVertexData(
        PositionOnlyBufferProvider<VoxelGeometryProvider>(
            voxelGeometryProvider));

    sceneManager->getRootSceneNode()->createChildSceneNode()->attachObject(
        renderableVoxelGrid);*/

    app.startEventLoop();
  }
  return 0;
}
