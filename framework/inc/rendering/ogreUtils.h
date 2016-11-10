#ifndef _FRAMEWORK_RENDERING_OGRE_UTILS_H_
#define _FRAMEWORK_RENDERING_OGRE_UTILS_H_

#include <OGRE/OgreSceneNode.h>
#include <OGRE/OgreEntity.h>

// Bunch of helper for the Ogre Rendering engine.
class OgreUtils {
 public:
  // These two picked off the Ogre forum. Credits: JDX_John:
  // http://www.ogre3d.org/forums/viewtopic.php?f=2&t=53647&start=0
  static void destroyAllAttachedMovableObjects(Ogre::SceneNode* node);
  static void destroySceneNode(Ogre::SceneNode* node);
  // Create a mesh instance and attach to passed in sceneNode.
  static Ogre::Entity* addMeshInstance(const std::string& meshName,
                                       Ogre::SceneNode* node);
};

#endif  //_FRAMEWORK_RENDERING_OGRE_UTILS_H_
