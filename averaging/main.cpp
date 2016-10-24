#include <iostream>
#include <memory>

#include <glog/logging.h>
#include <gflags/gflags.h>

#include <OGRE/Ogre.h>
#include <OGRE/OgreRectangle2D.h>
#include <OGRE/OgreConfigFile.h>

#include <cameraController.h>
#include <windowedRenderingApp.h>

#include <polyloop.h>
#include <polyloopGeometryProvider.h>
#include <positionOnlyBufferProvider.h>
#include <uniformVoxelGrid.h>
#include <uniformVoxelGridGeometryProvider.h>
#include <geometryRenderable.h>
#include <prefabs.h>

#include "separableGeometryInducedField.h"
#include "distanceFieldComputers.h"
#include "fieldSampler.h"
#include "levelSetMeshBuilder.h"
#include "triangleMeshGeometryProvider.h"

// Default flag values
DEFINE_bool(write_generated_level_set_mesh, false,
            "Should the mesh created by the level set mesh builder be written "
            "out to file?");

template <typename T>
using PositionRenderable = GeometryRenderable<PositionOnlyBufferProvider<T>>;

bool initScene(const std::string& windowName, const std::string& sceneName) {
  Ogre::Root* root = Ogre::Root::getSingletonPtr();
  Ogre::SceneManager* sceneManager =
      root->createSceneManager(Ogre::ST_GENERIC, sceneName);
  sceneManager->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));

  Ogre::Camera* mainCamera = sceneManager->createCamera("PrimaryCamera");
  Ogre::Viewport* viewport =
      root->getRenderTarget(windowName)->addViewport(mainCamera);
  viewport->setBackgroundColour(Ogre::ColourValue(1, 1, 1));

  mainCamera->setAspectRatio((float)viewport->getActualWidth() /
                             viewport->getActualHeight());
  mainCamera->setNearClipDistance(0.1);

  CameraController* cameraController =
      new CameraController("mainCamera", mainCamera);
  cameraController->setYawPitchDist(Ogre::Degree(0), Ogre::Degree(15), 10);
}

template <typename T>
using RenderBufferProvider = PositionOnlyBufferProvider<T>;
template <typename T>
using Renderable = GeometryRenderable<RenderBufferProvider<T>>;
template <typename T, typename U = DefaultMaterialPolicy<
                          DefaultRenderPolicy<RenderBufferProvider<T>>>>
using MaterialRenderable =
    GeometryRenderable<RenderBufferProvider<T>,
                       DefaultRenderPolicy<RenderBufferProvider<T>>, U>;

// View class that allows for rendering level set of height field
template <class Field>
class LevelSetMeshVisualizer {
 public:
  LevelSetMeshVisualizer(const Field& inducedField, Ogre::SceneNode* parent)
      : m_inducedField(&inducedField) {
    m_levelSetSceneNode = parent->createChildSceneNode();
    setLevel(2);
  }

  void setLevel(Kernel::FT value) {
    m_value = value;
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
    meshRenderable->setVertexData(
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

int main(int argc, char* argv[]) {
  google::InitGoogleLogging(argv[0]);

  WindowedRenderingApp app("Smoothing");

  Polyloop<CGAL::Point_3<Kernel>> p;
  buildPolyloopFromObj("data/loop1.obj", p);

  if (app.init(800, 800)) {
    initScene(app.getWindowName(), "PrimaryScene");

    // Add widgets
    CEGUI::WindowManager& windowManager = CEGUI::WindowManager::getSingleton();
    CEGUI::Window* rootWindow =
        windowManager.createWindow("DefaultWindow", "rootWin");
    CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(rootWindow);
    CEGUI::Window* levelSetWindow =
        windowManager.loadLayoutFromFile("AveragingMain.layout");
    rootWindow->addChild(levelSetWindow);

    Ogre::Root* root = Ogre::Root::getSingletonPtr();
    Ogre::SceneManager* sceneManager = root->getSceneManager("PrimaryScene");

    using LoopGeometryProvider =
        PolyloopGeometryProvider<CGAL::Point_3<Kernel>>;

    LoopGeometryProvider loopGeometryProvider(p);
    auto loopRenderable = new Renderable<LoopGeometryProvider>();
    loopRenderable->setVertexData(
        RenderBufferProvider<LoopGeometryProvider>(loopGeometryProvider));

    sceneManager->getRootSceneNode()->createChildSceneNode()->attachObject(
        loopRenderable);

    using Field = SeparableGeometryInducedField<SquaredDistanceFieldComputer>;
    Field inducedField;
    inducedField.addGeometry(p);
    auto meshVisualizer = new LevelSetMeshVisualizer<Field>(
        inducedField, sceneManager->getRootSceneNode());

    Ogre::SceneNode* planeNode =
        sceneManager->getRootSceneNode()->createChildSceneNode();
    planeNode->attachObject(getPrefab(Prefab::PLANE));
    planeNode->setScale(10, 10, 10);

    Ogre::SceneNode* axesNode =
        sceneManager->getRootSceneNode()->createChildSceneNode();
    axesNode->attachObject(getPrefab(Prefab::AXES));
    axesNode->setScale(1, 1, 1);

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
