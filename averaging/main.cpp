#include <iostream>
#include <memory>

#include <glog/logging.h>

#include <OGRE/Ogre.h>
#include <OGRE/OgreRectangle2D.h>
#include <OGRE/OgreConfigFile.h>
#include <CGAL/Point_3.h>

#include <cameraController.h>
#include <windowedRenderingApp.h>

#include <polyloop.h>
#include <polyloopGeometryProvider.h>
#include <positionOnlyBufferProvider.h>
#include <uniformVoxelGrid.h>
#include <uniformVoxelGridGeometryProvider.h>
#include <geometryRenderable.h>

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
  viewport->setBackgroundColour(Ogre::ColourValue(0.5, 0, 0));

  mainCamera->setAspectRatio((float)viewport->getActualWidth() /
                             viewport->getActualHeight());
  mainCamera->setPosition(Ogre::Vector3(0, 0, 200));
  mainCamera->lookAt(Ogre::Vector3(0, 0, 0));
  mainCamera->setNearClipDistance(5);

  CameraController* cameraController =
      new CameraController("mainCamera", mainCamera);
}

int main(int argc, char* argv[]) {
  google::InitGoogleLogging(argv[0]);

  WindowedRenderingApp app("Smoothing");

  Polyloop<CGAL::Point_3<Kernel>> p;
  p.addPoint(CGAL::Point_3<Kernel>(0, 0, 0));
  p.addPoint(CGAL::Point_3<Kernel>(100, 100, -100));
  p.addPoint(CGAL::Point_3<Kernel>(100, 0, -100));

  if (app.init(800, 800)) {
    initScene(app.getWindowName(), "PrimaryScene");

    Ogre::Root* root = Ogre::Root::getSingletonPtr();
    Ogre::SceneManager* sceneManager = root->getSceneManager("PrimaryScene");

    using LoopGeometryProvider =
        PolyloopGeometryProvider<CGAL::Point_3<Kernel>>;

    PositionRenderable<LoopGeometryProvider>* renderableLoop =
        new PositionRenderable<LoopGeometryProvider>();
    LoopGeometryProvider loopGeometryProvider(p);
    renderableLoop->setVertexData(
        PositionOnlyBufferProvider<LoopGeometryProvider>(loopGeometryProvider));

    sceneManager->getRootSceneNode()->createChildSceneNode()->attachObject(
        renderableLoop);

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
