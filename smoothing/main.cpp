#include <iostream>
#include <OGRE/Ogre.h>
#include <CGAL/Simple_cartesian.h>

int main(int argc, char* argv[]) {
  typedef CGAL::Simple_cartesian<float> Kernel;
  Kernel::Point_2 point1(0,0);
  Kernel::Point_2 point2(3,4);

  std::cout<<CGAL::squared_distance(point1, point2)<<std::endl;

  Ogre::Root *root = new Ogre::Root();
  root->initialise(false);
  Ogre::RenderWindow* renderWindow = root->createRenderWindow("Main", 800, 800, false);
  Ogre::SceneManager* sceneManager = root->createSceneManager(Ogre::ST_GENERIC);
  Ogre::Camera* mainCamera = sceneManager->createCamera("MainCamera");
  Ogre::Viewport* viewport = renderWindow->addViewport(mainCamera);

  root->startRendering();

  return 0;
}
