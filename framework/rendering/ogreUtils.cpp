#include <OGRE/OgreSceneManager.h>

#include "ogreUtils.h"

using SceneNode = Ogre::SceneNode;

void OgreUtils::destroyAllAttachedMovableObjects(SceneNode* node) {
  if (!node) return;

  // Destroy all the attached objects
  SceneNode::ObjectIterator itObject = node->getAttachedObjectIterator();

  while (itObject.hasMoreElements())
    node->getCreator()->destroyMovableObject(itObject.getNext());

  // Recurse to child SceneNodes
  SceneNode::ChildNodeIterator itChild = node->getChildIterator();

  while (itChild.hasMoreElements()) {
    SceneNode* pChildNode = static_cast<SceneNode*>(itChild.getNext());
    destroyAllAttachedMovableObjects(pChildNode);
  }
}

void OgreUtils::destroySceneNode(SceneNode* node) {
  if (!node) return;
  destroyAllAttachedMovableObjects(node);
  node->removeAndDestroyAllChildren();
  node->getCreator()->destroySceneNode(node);
}
