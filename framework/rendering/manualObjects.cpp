#include <OGRE/Ogre.h>

#include "manualObjects.h"

namespace {
// TODO msati3: Make the creation API's work with multiple scene managers?
Ogre::SceneManager* getSceneManager() {
  return Ogre::Root::getSingletonPtr()
      ->getSceneManagerIterator()
      .begin()
      ->second;
}

const char* AXES_OBJECT_NAME = "Axes";
const char* PLANE_OBJECT_NAME = "Plane";
}  // End anon namespace

// TODO msati3: This has to morph into a Prefab library where primitive manual
// objects are created lazily and registered
//
// Create canonical coordinate axes
Ogre::ManualObject* createCoordinateAxes() {
  Ogre::SceneManager* sceneManager = getSceneManager();
  Ogre::ManualObject* axis = sceneManager->createManualObject(AXES_OBJECT_NAME);
  axis->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_LINE_LIST);
  axis->position(0, 0, 0);
  axis->colour(1, 0, 0);
  axis->position(1, 0, 0);
  axis->position(0, 0, 0);
  axis->colour(0, 1, 0);
  axis->position(0, 1, 0);
  axis->position(0, 0, 0);
  axis->colour(0, 0, 1);
  axis->position(0, 0, 1);
  axis->end();
  return axis;
}

// Create canonical plane, cream colored
Ogre::ManualObject* createPlane() {
  Ogre::SceneManager* sceneManager = getSceneManager();
  Ogre::ManualObject* plane = sceneManager->createManualObject(PLANE_OBJECT_NAME);
  plane->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_TRIANGLE_STRIP);
  plane->colour(1, 1, 0.81);
  plane->position(0, 1, 0);
  plane->position(0, 0, 0);
  plane->position(1, 1, 0);
  plane->position(1, 0, 0);
  plane->end();
  return plane;
}
