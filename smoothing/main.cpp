#include <iostream>
#include <memory>

#include <glog/logging.h>

#include <OGRE/Ogre.h>
#include <OGRE/OgreRectangle2D.h>
#include <OGRE/OgreConfigFile.h>
#include <CGAL/Point_3.h>

#include "cameraController.h"
#include "windowedRenderingApp.h"

#include "polyloop.h"
#include "polyloopGeometryProvider.h"
#include "uniformVoxelGrid.h"
#include "uniformVoxelGridGeometryProvider.h"
#include "sequentialGeometryRenderable.h"

bool initScene(const std::string& windowName, const std::string& sceneName) {
  Ogre::Root* root = Ogre::Root::getSingletonPtr();
  Ogre::SceneManager* sceneManager =
      root->createSceneManager(Ogre::ST_GENERIC, sceneName);
  sceneManager->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));

  Ogre::ConfigFile cf;
  cf.load("resources.cfg");

  Ogre::ConfigFile::SectionIterator secIt = cf.getSectionIterator();
  while (secIt.hasMoreElements()) {
    Ogre::ConfigFile::SettingsMultiMap* settings = secIt.getNext();
    for (auto iter = settings->begin(); iter != settings->end(); ++iter) {
      Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
          iter->second, iter->first);
    }
  }
  Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

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

    SequentialGeometryRenderable<LoopGeometryProvider>* renderableLoop =
        new SequentialGeometryRenderable<LoopGeometryProvider>();
    renderableLoop->setRenderData(LoopGeometryProvider(p));

    sceneManager->getRootSceneNode()->createChildSceneNode()->attachObject(
        renderableLoop);

    UniformVoxelGrid voxelGrid(100, 10);

    using VoxelGeometryProvider =
        UniformVoxelGridGeometryProvider<VoxelGridCubeProvider>;
    SequentialGeometryRenderable<VoxelGeometryProvider>* renderableVoxelGrid =
        new SequentialGeometryRenderable<VoxelGeometryProvider>();
    renderableVoxelGrid->setRenderData(VoxelGeometryProvider(voxelGrid));

    sceneManager->getRootSceneNode()->createChildSceneNode()->attachObject(
        renderableVoxelGrid);

    app.startEventLoop();
  }
  return 0;
}
