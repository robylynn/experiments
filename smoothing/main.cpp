#include <iostream>
#include <memory>

#include <glog/logging.h>

#include <OGRE/Ogre.h>
#include <OGRE/OgreRectangle2D.h>
#include <CGAL/Point_3.h>

#include "windowedRenderingApp.h"
#include "polyloop.h"
#include "polyloopRenderable.h"

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
  mainCamera->setNearClipDistance(5);
}

int main(int argc, char* argv[]) {
  google::InitGoogleLogging(argv[0]);

  WindowedRenderingApp app("Smoothing");
  if (app.init(800, 800)) {
    initScene(app.getWindowName(), "PrimaryScene");

    Polyloop<CGAL::Point_3<Kernel>> p;
    p.addPoint(CGAL::Point_3<Kernel>(0, 0, 0));
    p.addPoint(CGAL::Point_3<Kernel>(100, 100, -100));

    Ogre::SceneNode* polyloopNode = Ogre::Root::getSingleton()
                                        .getSceneManager("PrimaryScene")
                                        ->getRootSceneNode()
                                        ->createChildSceneNode("PolyloopNode");
    polyloopNode->attachObject(
        new PolyloopRenderable<CGAL::Point_3<Kernel>>(p));

    Ogre::Root::getSingleton().startRendering();
  }

  return 0;
}
