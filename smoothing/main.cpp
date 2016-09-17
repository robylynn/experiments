#include <iostream>
#include <memory>
#include <glog/logging.h>

#include <OGRE/Ogre.h>
#include <OGRE/OgreRectangle2D.h>
#include <CGAL/Point_3.h>

#include "polyloop.h"
#include "polyloopRenderable.h"

bool initRenderSystem() {
  Ogre::Root* root = new Ogre::Root();
  if (!(root->showConfigDialog())) return false;
  root->initialise(false);

  Ogre::RenderWindow* renderWindow =
      root->createRenderWindow("Example", 800, 800, false);
  Ogre::SceneManager* sceneManager =
      root->createSceneManager(Ogre::ST_GENERIC, "PrimarySceneManager");
  sceneManager->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));

  Ogre::Camera* mainCamera = sceneManager->createCamera("PrimaryCamera");
  Ogre::Viewport* viewport = renderWindow->addViewport(mainCamera);
  viewport->setBackgroundColour(Ogre::ColourValue(0.5, 0, 0));

  mainCamera->setAspectRatio((float)viewport->getActualWidth() /
                             viewport->getActualHeight());
  mainCamera->setNearClipDistance(5);
}

int main(int argc, char* argv[]) {
  google::InitGoogleLogging(argv[0]);

  initRenderSystem();
  Polyloop<CGAL::Point_3<Kernel>> p;
  p.addPoint(CGAL::Point_3<Kernel>(0, 0, 0));
  p.addPoint(CGAL::Point_3<Kernel>(100, 100, -100));

  Ogre::SceneNode* polyloopNode = Ogre::Root::getSingleton()
                                  .getSceneManager("PrimarySceneManager")
                                  ->getRootSceneNode()
                                  ->createChildSceneNode("PolyloopNode");
  polyloopNode->attachObject(new PolyloopRenderable<CGAL::Point_3<Kernel>>(p));
  LOG(INFO) << "Created scene";

  Ogre::Root::getSingleton().startRendering();

  return 0;
}
