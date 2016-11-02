#ifndef _AVERAGING_POLYLOOPS_VIEW_H_
#define _AVERAGING_POLYLOOPS_VIEW_H_

#include <OGRE/OgreSceneNode.h>

// Allows looking at averages of lines
class AveragingPolyloopsView {
 public:
  AveragingPolyloopsView(Ogre::SceneNode* rootNode);

 private:
  Ogre::SceneNode* m_rootNode;
};

#endif  //_AVERAGING_POLYLOOPS_VIEW_H_
