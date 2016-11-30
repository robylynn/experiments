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

constexpr int NUM_DATASETS = 2;

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
      Kernel::Line_2 line;
      file >> line;
      lines.push_back(line);
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
  m_squaredDistField.reset(new SquaredDistField_2());
  std::vector<Kernel::Point_2> points;
  std::vector<Kernel::Line_2> lines;
  std::string dataSetName =
      "data/dataset" + std::to_string(m_dataSetIndex) + "_2D.obj";

  readDataSet(dataSetName, points, lines);
  m_dataSetIndex = ++m_dataSetIndex == NUM_DATASETS? 0 : m_dataSetIndex;

  for (const auto& point : points) {
    m_squaredDistField->addGeometry(point);
  }
  for (const auto& line : lines) {
    m_squaredDistField->addGeometry(line);
  }
}
