#ifndef _FRAMEWORK_RENDERING_SELECTIONMANAGER_H_
#define _FRAMEWORK_RENDERING_SELECTIONMANAGER_H_

#include <OGRE/OgreCamera.h>
#include <OGRE/OgreSceneQuery.h>
#include <OGRE/OgreMovableObject.h>

#include "selectableObject.h"

// Handles selection of objects based on rayshooting from the camera's
// viewpoint. This is done thru a SelectableObject interface. Any object that
// needs to provide selection functionality, will be a SelectableObject. A
// selectable object must contain a movable object that describes the
// SelectableObject within Ogre's scenegraph. Additionally, the
// SelectableObject can implement any of the mouse and keyboard callbacks. The
// SelectionManager forwards these calls to the current selected object.
//
// For nuanced selection, SelectableObjects must also provide for a
// queryDistance method that returns the closest distance from a ray to the
// object. The SelectableObject also provides a setSelected function that the
// SelectionManager uses to notify if of selection. The queryDistance call can
// cache information that will be useful for handling a setSelected call if so
// desired.
class SelectionManager {
 public:
  SelectionManager() : m_camera(nullptr), m_selectedObject(nullptr) {}

  void setCamera(const Ogre::Camera& camera) { m_camera = &camera; }

  void addSelectableObject(ISelectableObject* object) {
    m_selectableObjectsMap.insert(
        std::make_pair(object->movableObject(), object));
  }

  void selectionQuery(float mouseX, float mouseY);

 private:
  const Ogre::Camera* m_camera;
  std::map<Ogre::MovableObject*, ISelectableObject*> m_selectableObjectsMap;
  ISelectableObject* m_selectedObject;
};

#endif  // _FRAMEWORK_RENDERING_SELECTIONMANAGER_H_
