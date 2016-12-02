#include <fstream>

#include <OGRE/OgreEntity.h>
#include <OGRE/OgreSceneManager.h>

#include <CGAL/Arr_linear_traits_2.h>
#include <CGAL/Arrangement_2.h>
#include <CGAL/Segment_2.h>
#include <CGAL/Sweep_line_2_algorithms.h>

#include <appContext.h>
#include <dynamicMeshManager.h>
#include <polyloop_2.h>

#include "averagingPolyloops_2View.h"

constexpr int NUM_DATASETS = 5;

namespace Context = Framework::AppContext;

namespace {
bool readDataSet(const std::string& datasetName,
                 std::vector<Kernel::Point_2>& points,
                 std::vector<Kernel::Line_2>& lines) {
  std::fstream file(datasetName);
  if (!file.good()) {
    LOG(ERROR) << "File handle not accessible for reading 2D geometry "
                  "configuration dataset "
               << datasetName;
    return false;
  }

  std::string word;
  while (file >> word) {
    if (word == "Point") {
      Kernel::Point_2 point;
      file >> point;
      points.push_back(point);
    } else if (word == "Line") {
      Kernel::Point_2 point1;
      Kernel::Point_2 point2;
      file >> point1;
      file >> point2;
      lines.push_back(Kernel::Line_2(point1, point2));
    }
  }
  return true;
}
}  // end anonymous namespace

AveragingPolyloops_2View::AveragingPolyloops_2View(Ogre::SceneNode* rootNode)
    : m_dataSetIndex(0),
      m_rootNode(rootNode),
      m_polyloopsRootNode(m_rootNode->createChildSceneNode()) {}

void AveragingPolyloops_2View::populateData() {
  m_pointsGeometry.clear();
  m_linesGeometry.clear();
  m_squaredDistField.reset(new SquaredDistField_2());
  DynamicMeshManager& dynamicMeshManager =
      Framework::AppContext::getDynamicMeshManager();

  std::string dataSetName =
      "data/dataset" + std::to_string(m_dataSetIndex) + "_2D.obj";

  readDataSet(dataSetName, m_pointsGeometry, m_linesGeometry);
  m_dataSetIndex = ++m_dataSetIndex == NUM_DATASETS ? 0 : m_dataSetIndex;

  for (const auto& point : m_pointsGeometry) {
    m_squaredDistField->addGeometry(point);
    /*std::vector<Kernel::Point_2> pointVec{point};
    Ogre::Entity* pointEntity =
        dynamicMeshManager.addMesh(pointVec, m_polyloopsRootNode);
    pointEntity->setMaterialName("Materials/GeometryPoint");*/
  }
  for (const auto& line : m_linesGeometry) {
    m_squaredDistField->addGeometry(line);
    /*std::vector<Kernel::Point_2> lineVec{
        line.projection(Kernel::Point_2(-10, -10)),
        line.projection(Kernel::Point_2(10, 10))};
    Ogre::Entity* lineEntity =
        dynamicMeshManager.addMesh(lineVec, m_polyloopsRootNode);
    lineEntity->setMaterialName("Materials/GeometryLine");*/
  }
}
