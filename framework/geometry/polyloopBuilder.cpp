#include <fstream>

#include <glog/logging.h>

#include <boost/tokenizer.hpp>

#include <CGAL/Point_3.h>
#include <CGAL/Polygon_2.h>
#include <CGAL/bounding_box.h>
#include "polyloop_3.h"

// Handle , and space delimited files for now.
constexpr const char* const COORDINATE_DELIMITERS = ", ";

// Creates a polyloop from an obj file
bool buildPolyloopFromObj(const std::string& filePath, Polyloop_3& polyloop) {
  std::ifstream file(filePath);

  if (!file.good()) {
    LOG(ERROR) << "File handle not accesible for building polyloop from file "
               << filePath;
    return false;
  }

  bool fClosedLoop = false;
  std::string word;

  int startIndex = -1;
  size_t lastIndex = 0;
  size_t nextIndex;

  // Do not add the last point, which is repeated, and implicitly represented
  // in the polyloop representation.
  Kernel::Point_3 lastPoint;
  size_t numVertices = 0;
  while (file >> word) {
    if (word == "v") {
      Kernel::Point_3 point;
      file >> point;
      if (numVertices++ != 0) {
        polyloop.addPoint(lastPoint);
      }
      lastPoint = point;
    } else if (word == "l") {
      if (fClosedLoop) {
        LOG(ERROR) << "Encountered additional segment after loop was closed";
        return false;
      }
      file >> nextIndex;
      if (startIndex == -1) {
        startIndex = nextIndex;
        lastIndex = startIndex;
      }
      if (nextIndex != lastIndex) {
        LOG(ERROR) << "Curve in obj file skips an index (last, next) indices: "
                   << lastIndex << "," << nextIndex;
        return false;
      }
      file >> nextIndex;
      if (nextIndex != ++lastIndex) {
        if (nextIndex == startIndex) {
          fClosedLoop = true;
        } else {
          LOG(ERROR)
              << "Curve in obj file is not ordered (last, next) indices: "
              << lastIndex - 1 << "," << nextIndex;
          return false;
        }
      }
    }
  }

  LOG_IF(ERROR, !fClosedLoop) << "Polyloop is not closed";
  return fClosedLoop;
}

// A vertex list file format consists of a sequence of vertices, each line
// consisting of delimited coordinates of a vertex. The polyloop is assumed to
// be the loop created by connecting consecutive vertices in the file. The last
// and first points are implicitly assumed to be connected (the vertex list
// file should not close the loop itself).
bool buildPolyloopFromVertexList(const std::string& filePath,
                                 Polyloop_3& polyloop) {
  std::ifstream file(filePath);

  if (!file.good()) {
    LOG(ERROR) << "File handle not accesible for building polyloop from file "
               << filePath;
    return false;
  }

  std::string line;
  boost::char_separator<char> separator(COORDINATE_DELIMITERS);
  // First, identify the format of the file by reading a single line.
  while (std::getline(file, line)) {
    std::array<Kernel::FT, 3> pointCoords{0, 0, 0};
    boost::tokenizer<boost::char_separator<char>> tokenizer(line, separator);
    int index = 0;
    for (const auto& coord : tokenizer) {
      if (index > pointCoords.size()) {
        LOG(ERROR) << "Could not build polyloop from file -- file not in "
                      "vertexlist format";
        return false;
      }
      pointCoords[index++] = std::stod(coord);
    }
    polyloop.addPoint(
        Kernel::Point_3(pointCoords[0], pointCoords[1], pointCoords[2]));
  }
  return true;
}
