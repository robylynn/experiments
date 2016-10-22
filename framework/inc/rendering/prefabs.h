#ifndef _FRAMEWORK_RENDERING_PREFABS_H_
#define _FRAMEWORK_RENDERING_PREFABS_H_

#include <OGRE/OgreManualObject.h>

// Provides a bunch of pre-fab shapes. These are lazily created when the user
// requests for them, and then remain in memory.

enum class Prefab : unsigned int {
  AXES,
  PLANE,
};

Ogre::ManualObject* getPrefab(const Prefab& prefab);

#endif  //_FRAMEWORK_RENDERING_PREFABS_H_
