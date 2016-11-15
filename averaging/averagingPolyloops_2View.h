#ifndef _AVERAGING_POLYLOOPS_2_VIEW_H_
#define _AVERAGING_POLYLOOPS_2_VIEW_H_

#include <OGRE/OgreSceneNode.h>

#include "commonViewInterface.h"

// Allows looking at averages of lines
class AveragingPolyloops_2View : public IAveragingView {
 public:
  AveragingPolyloops_2View(Ogre::SceneNode* rootNode);
  const SquaredDistField* getGeometryInducedField() {
    return m_squaredDistField.get();
  }
  void populateData();

 private:
  Ogre::SceneNode* m_rootNode;
  Ogre::SceneNode* m_polyloopsRootNode;
  std::unique_ptr<SquaredDistField> m_squaredDistField;
};

#endif  //_AVERAGING_POLYLOOPS_2_VIEW_H_
