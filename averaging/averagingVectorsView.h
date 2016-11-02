#ifndef _AVERAGING_VECTORS_VIEW_H_
#define _AVERAGING_VECTORS_VIEW_H_

#include <OGRE/OgreSceneNode.h>

// Allows looking at averages of lines
class AveragingVectorsView {
 public:
  AveragingVectorsView(Ogre::SceneNode* rootNode);
 private:
  Ogre::SceneNode* m_rootNode;
};

#endif  //_AVERAGING_VECTORS_VIEW_H_
