#include <fstream>
#include <iostream>
#include <memory>

#include <glog/logging.h>
#include <gflags/gflags.h>

#include <OGRE/Ogre.h>
#include <OGRE/OgreRectangle2D.h>
#include <OGRE/OgreConfigFile.h>

#include <cameraController.h>
#include <windowedRenderingApp.h>

#include <polyloop_3.h>
#include <defaultRenderables.h>
#include <uniformVoxelGrid.h>
#include <uniformVoxelGridGeometryProvider.h>
#include <prefabs.h>

DEFINE_string(
    polyloop_file,
    "/home/mukul/development/experiments/approximation/data/simplification/"
    "refined/curves0",
    "Name of the file from which the polyloop to be visualized will be loaded");

bool initScene(const std::string& windowName, const std::string& sceneName) {
  Ogre::Root* root = Ogre::Root::getSingletonPtr();
  Ogre::SceneManager* sceneManager =
      root->createSceneManager(Ogre::ST_GENERIC, sceneName);
  sceneManager->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));

  Ogre::Camera* mainCamera = sceneManager->createCamera("PrimaryCamera");
  Ogre::Viewport* viewport =
      root->getRenderTarget(windowName)->addViewport(mainCamera);
  viewport->setBackgroundColour(Ogre::ColourValue(0.5, 0.5, 0.5));

  mainCamera->setNearClipDistance(0.5);
}

int main(int argc, char* argv[]) {
  google::InitGoogleLogging(argv[0]);
  google::ParseCommandLineFlags(&argc, &argv, true);

  WindowedRenderingApp app("Smoothing");

  Polyloop_3 p;
  if (!buildPolyloopFromVertexList(FLAGS_polyloop_file, p)) {
    return -1;
  }

  if (app.init(1200, 900)) {
    initScene(app.getWindowName(), "PrimaryScene");

    Ogre::Root* root = Ogre::Root::getSingletonPtr();
    Ogre::SceneManager* sceneManager = root->getSceneManager("PrimaryScene");

    make_mesh_renderable(p, "loop");
    Ogre::Entity* loopEntity = sceneManager->createEntity("loop");
    Ogre::SceneNode* loopNode =
        sceneManager->getRootSceneNode()->createChildSceneNode();
    loopNode->attachObject(loopEntity);
    loopNode->showBoundingBox(true);

    // Switch camera to center of polyloop
    Ogre::Camera* mainCamera = sceneManager->getCamera("PrimaryCamera");
    app.getSelectionManager().setCamera(*mainCamera);
    sceneManager->_updateSceneGraph(mainCamera);

    Ogre::Vector3 extents = loopEntity->getBoundingBox().getMaximum() -
                            loopEntity->getBoundingBox().getMinimum();
    CameraController* cameraController =
        new CameraController("PrimaryCameraController", mainCamera);
    cameraController->setTarget(loopNode);

    Ogre::SceneNode* axesNode =
        sceneManager->getRootSceneNode()->createChildSceneNode();
    axesNode->attachObject(getPrefab(Prefab::AXES));
    axesNode->setScale(10, 10, 10);

    app.startEventLoop();
  }
  return 0;
}
