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
#include <polyloopGeometryProvider.h>
#include <singleElementBufferProvider.h>
#include <uniformVoxelGrid.h>
#include <uniformVoxelGridGeometryProvider.h>
#include <geometryRenderable.h>
#include <prefabs.h>

DEFINE_string(
    polyloop_file,
    "/home/mukul/development/experiments/approximation/data/simplification/"
    "refined/curves0",
    "Name of the file from which the polyloop to be visualized will be loaded");

DEFINE_bool(twod, false, "Is the file of 2D coordinates");

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
  mainCamera->setPosition(Ogre::Vector3(0, 0, 200));
  mainCamera->lookAt(Ogre::Vector3(0, 0, 0));
  mainCamera->setNearClipDistance(0.5);

  CameraController* cameraController =
      new CameraController("mainCamera", mainCamera);
}

template <typename T>
using RenderBufferProvider = PositionOnlyBufferProvider<T>;
template <typename T>
using Renderable = GeometryRenderable<RenderBufferProvider<T>>;

int main(int argc, char* argv[]) {
  google::InitGoogleLogging(argv[0]);
  google::ParseCommandLineFlags(&argc, &argv, true);

  WindowedRenderingApp app("Smoothing");

  Polyloop_3 p;
  std::ifstream ffile;
  std::cout << "Reading polyloop from " << FLAGS_polyloop_file << std::endl;
  ffile.open(FLAGS_polyloop_file);

  float x = 0, y = 0, z = 0;
  while (!ffile.eof()) {
    ffile >> x;
    ffile >> y;
    if (!FLAGS_twod) {
      ffile >> z;
    }
    p.addPoint(CGAL::Point_3<Kernel>(x, y, z));
  }

  if (app.init(800, 800)) {
    initScene(app.getWindowName(), "PrimaryScene");

    Ogre::Root* root = Ogre::Root::getSingletonPtr();
    Ogre::SceneManager* sceneManager = root->getSceneManager("PrimaryScene");

    using LoopGeometryProvider =
        PolyloopGeometryProvider<Polyloop_3>;

    LoopGeometryProvider loopGeometryProvider(p);
    auto loopRenderable = new Renderable<LoopGeometryProvider>();
    loopRenderable->setVertexBufferData(
        RenderBufferProvider<LoopGeometryProvider>(loopGeometryProvider));
    sceneManager->getRootSceneNode()->createChildSceneNode()->attachObject(
        loopRenderable);

    Ogre::SceneNode* axesNode =
        sceneManager->getRootSceneNode()->createChildSceneNode();
    axesNode->attachObject(getPrefab(Prefab::AXES));
    axesNode->setScale(10, 10, 10);

    app.startEventLoop();
  }
  return 0;
}
