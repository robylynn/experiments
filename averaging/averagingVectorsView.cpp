#include <OGRE/OgreSceneManager.h>
#include <OGRE/OgreEntity.h>
#include <OGRE/OgreSubEntity.h>

#include <CGAL/Point_3.h>
#include <CGAL/Vector_3.h>
#include <CGAL/Origin.h>
#include <CGAL/Segment_3.h>

#include <Eigen/Dense>

#include <geometryTypes.h>
#include <defaultRenderables.h>
#include <ogreUtils.h>

#include "averagingVectorsView.h"

constexpr int NUM_POINTS = 2;

namespace {
using namespace Eigen;

// The average minimizes the distances (L2 norm) between itself and the given
// poitns.
Kernel::Point_3 averageL2Min(std::vector<Kernel::Point_3> points) {
  Kernel::Vector_3 output;
  for (auto point : points) {
    output = output + (point - CGAL::ORIGIN);
  }
  output = output / points.size();
  return CGAL::ORIGIN + output;
}

Kernel::Point_3 averageProjMax(std::vector<Kernel::Point_3> points) {
  MatrixXf pointMatrix(points.size(), 3);
  for (int i = 0; i < points.size(); ++i) {
    pointMatrix.row(i) << points[i].x(), points[i].y(), points[i].z();
    // cout << "Its singular values are:" << endl
    //   << svd.singula
  }
  JacobiSVD<MatrixXf> svd(pointMatrix, ComputeThinU | ComputeThinV);
  auto firstSingularVector = svd.matrixV().col(0);
  Kernel::Point_3 output(firstSingularVector[0], firstSingularVector[1],
                         firstSingularVector[2]);
  return output;
}
}  // end anon namespace

AveragingVectorsView::AveragingVectorsView(Ogre::SceneNode* rootNode)
    : m_rootNode(rootNode),
      m_vectorsRootNode(nullptr),
      m_spherePointsGenerator(1) {
  Ogre::SceneNode* sphereNode = m_rootNode->createChildSceneNode();
  Ogre::Entity* sphereEntity = m_rootNode->getCreator()->createEntity(
      "vectorSphere", Ogre::SceneManager::PT_SPHERE);
  sphereEntity->setMaterialName("Materials/IllustrativeObjectsTransparent");
  sphereNode->attachObject(sphereEntity);
  sphereNode->setScale(Ogre::Vector3(0.02, 0.02, 0.02));
}

void AveragingVectorsView::populateData() {
  // Kill off initial scene node content
  OgreUtils::destroySceneNode(m_vectorsRootNode);
  const std::string BASE_NAME = "vector";

  m_vectorsRootNode = m_rootNode->createChildSceneNode();
  std::vector<Kernel::Point_3> points;
  std::copy_n(m_spherePointsGenerator, NUM_POINTS, std::back_inserter(points));

  int count = 0;
  for (auto point : points) {
    std::string meshName = BASE_NAME + std::to_string(count);
    std::vector<Kernel::Point_3> vector{Kernel::Point_3(0, 0, 0), point};
    auto vMesh = make_mesh_renderable(vector, meshName);
    Ogre::Entity* vEntity = m_rootNode->getCreator()->createEntity(meshName);
    vEntity->setMaterialName("Materials/DirectedVectors");
    m_vectorsRootNode->attachObject(vEntity);
    ++count;
  }

  // Here come the averages
  std::vector<Kernel::Point_3> l2Average{Kernel::Point_3(0, 0, 0),
                                         averageL2Min(points)};
  (void)make_mesh_renderable(l2Average, "l2MinAverage");
  Ogre::Entity* l2Entity =
      m_rootNode->getCreator()->createEntity("l2MinAverage");
  l2Entity->setMaterialName("Materials/SimpleAverage");
  m_vectorsRootNode->attachObject(l2Entity);

  std::vector<Kernel::Point_3> projMaxAverage{Kernel::Point_3(0, 0, 0),
                                              averageProjMax(points)};
  (void)make_mesh_renderable(projMaxAverage, "projMaxAverage");
  Ogre::Entity* projEntity =
      m_rootNode->getCreator()->createEntity("projMaxAverage");
  projEntity->setMaterialName("Materials/ProjAverage");
  m_vectorsRootNode->attachObject(projEntity);
}
