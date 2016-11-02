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

#include <defaultRenderables.h>
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
#include "viewManager.h"

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


int main(int argc, char* argv[]) {
  google::InitGoogleLogging(argv[0]);

  WindowedRenderingApp app("Smoothing");

  if (app.init(1200, 900)) {
    initScene(app, "PrimaryScene");

    Ogre::Root* root = Ogre::Root::getSingletonPtr();
    Ogre::SceneManager* sceneManager = root->getSceneManager("PrimaryScene");
    ViewManager viewManager(sceneManager);
    viewManager.init();

    /*using Field = SeparableGeometryInducedField<Kernel::Point_3,
                                                SquaredDistanceFieldComputer>;
    Field inducedField;
    inducedField.addGeometry(p);
    auto meshVisualizer = new LevelSetMeshVisualizer<Field>(
        inducedField, sceneManager->getRootSceneNode());

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
    planarDistanceFieldVisualizer->addToScene();*/

    /*Ogre::SceneNode* planeNode =
        sceneManager->getRootSceneNode()->createChildSceneNode();
    planeNode->attachObject(getPrefab(Prefab::PLANE));
    planeNode->setScale(10, 10, 10);*/

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
