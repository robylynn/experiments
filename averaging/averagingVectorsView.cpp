#include <OGRE/OgreSceneManager.h>
#include <OGRE/OgreEntity.h>
#include <OGRE/OgreSubEntity.h>

#include <CGAL/Point_3.h>
#include <CGAL/Vector_3.h>
#include <CGAL/Origin.h>
#include <CGAL/Segment_3.h>

#include <Eigen/Dense>

#include <appContext.h>
#include <dynamicMeshManager.h>
#include <geometryTypes.h>
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
  // m_squaredDistField.reset(new SquaredDistField());
  DynamicMeshManager& dynamicMeshManager =
      Framework::AppContext::getDynamicMeshManager();

  m_vectorsRootNode = m_rootNode->createChildSceneNode();
  std::vector<Kernel::Point_3> points;
  std::copy_n(m_spherePointsGenerator, NUM_POINTS, std::back_inserter(points));

  for (auto point : points) {
    std::vector<Kernel::Point_3> vPoints{Kernel::Point_3(0, 0, 0), point};
    // m_squaredDistField->addGeometry(Kernel::Segment_3(vPoints[0],
    // vPoints[1]));

    Ogre::Entity* vEntity =
        dynamicMeshManager.addMesh(vPoints, m_vectorsRootNode);
    vEntity->setMaterialName("Materials/DirectedVectors");
  }

  // Here come the averages
  std::vector<Kernel::Point_3> l2Average{Kernel::Point_3(0, 0, 0),
                                         averageL2Min(points)};
  Ogre::Entity* l2Entity =
      dynamicMeshManager.addMesh(l2Average, m_vectorsRootNode);
  l2Entity->setMaterialName("Materials/SimpleAverage");

  std::vector<Kernel::Point_3> projMaxAverage{Kernel::Point_3(0, 0, 0),
                                              averageProjMax(points)};
  Ogre::Entity* projEntity =
      dynamicMeshManager.addMesh(projMaxAverage, m_vectorsRootNode);
  projEntity->setMaterialName("Materials/ProjAverage");
}
