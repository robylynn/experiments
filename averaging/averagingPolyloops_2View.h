#ifndef _AVERAGING_POLYLOOPS_2_VIEW_H_
#define _AVERAGING_POLYLOOPS_2_VIEW_H_

#include <OGRE/OgreSceneNode.h>

#include "commonViewInterface.h"

class AveragingPolyloops_2View : public IAveragingView {
 public:
  AveragingPolyloops_2View(Ogre::SceneNode* rootNode);
  const SquaredDistField_2* getGeometryInducedField() {
    return m_squaredDistField.get();
  }
  void populateData();

 private:
  size_t m_dataSetIndex;
  Ogre::SceneNode* m_rootNode;
  Ogre::SceneNode* m_polyloopsRootNode;
  std::unique_ptr<SquaredDistField_2> m_squaredDistField;
  std::vector<Kernel::Point_2> m_pointsGeometry;
  std::vector<Kernel::Line_2> m_linesGeometry;
};

#endif  //_AVERAGING_POLYLOOPS_2_VIEW_H_
