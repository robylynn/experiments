#ifndef _AVERAGING_LINES_VIEW_H_
#define _AVERAGING_LINES_VIEW_H_

#include <OGRE/OgreSceneNode.h>

//Allows looking at averages of lines
class AveragingLinesView {
  public:
    AveragingLinesView(Ogre::SceneNode* rootNode);
  private:
    Ogre::SceneNode* m_rootNode;
};


#endif //_AVERAGING_LINES_VIEW_H_
