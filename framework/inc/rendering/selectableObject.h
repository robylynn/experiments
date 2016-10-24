#ifndef _FRAMEWORK_RENDERING_SELECTABLEOBJECT_H_
#define _FRAMEWORK_RENDERING_SELECTABLEOBJECT_H_

#include <OGRE/OgreMovableObject.h>
#include <OGRE/OgreRay.h>

// The interface any selectable object must implement
class ISelectableObject {
 public:
  virtual ~ISelectableObject() {}
  virtual float queryDistance(const Ogre::Ray& ray) = 0;
  virtual Ogre::MovableObject* movableObject() const = 0;
  virtual void setSelected(const Ogre::Ray& ray) = 0;
};

#endif //_FRAMEWORK_RENDERING_SELECTABLEOBJECT_H_
