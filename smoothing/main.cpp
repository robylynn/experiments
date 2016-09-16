#include <iostream>
#include <memory>

#include <OGRE/Ogre.h>
#include <CGAL/Point_3.h>

#include "polyloop.h"

bool initRenderSystem() {
  std::unique_ptr<Ogre::Root> root(new Ogre::Root());
  if (!(root->showConfigDialog()))
    return false;
  root->initialise(false);

  Ogre::RenderWindow* renderWindow = root->createRenderWindow("Example", 800,
      800, false);
  Ogre::SceneManager* sceneManager = root->createSceneManager(Ogre::ST_GENERIC,
      "PrimarySceneManager");
  Ogre::Camera* mainCamera = sceneManager->createCamera("PrimaryCamera");
  Ogre::Viewport* viewport = renderWindow->addViewport(mainCamera);
  mainCamera->setAspectRatio((float)viewport->getActualWidth() /
      viewport->getActualHeight());
  mainCamera->setNearClipDistance(5);
}

int main(int argc, char* argv[]) {
  initRenderSystem();
  Polyloop<CGAL::Point_3<Kernel>> p;
  p.addPoint(CGAL::Point_3<Kernel>(0,0,0));

  Ogre::Root::getSingleton().getSceneManager("PrimarySceneManager");
  Ogre::Root::getSingleton().startRendering();

  return 0;
}
