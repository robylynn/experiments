#ifndef _AVERAGING_VECTORS_VIEW_H_
#define _AVERAGING_VECTORS_VIEW_H_

#include <CGAL/point_generators_3.h>

#include <OGRE/OgreSceneNode.h>

#include <geometryTypes.h>

#include "commonViewInterface.h"

// Allows looking at averages of lines
class AveragingVectorsView : public IAveragingView {
 public:
  AveragingVectorsView(Ogre::SceneNode* rootNode);
  void populateData();
  const SquaredDistField* getGeometryInducedField() {
    return m_squaredDistField.get();
  }

 private:
  Ogre::SceneNode* m_rootNode;
  Ogre::SceneNode* m_vectorsRootNode;
  CGAL::Random_points_on_sphere_3<Kernel::Point_3> m_spherePointsGenerator;
  std::unique_ptr<SquaredDistField> m_squaredDistField;
};

#endif  //_AVERAGING_VECTORS_VIEW_H_
