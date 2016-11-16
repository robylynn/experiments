#include <fstream>

#include <glog/logging.h>

#include <boost/tokenizer.hpp>

#include "polyline_3.h"

// Handle , and space delimited files for now.
constexpr const char* const COORDINATE_DELIMITERS = ", ";

// Creates a polyline from an obj file
bool buildPolylineFromObj(const std::string& filePath,
                          GeometryPolyline_3& polyline) {
  std::ifstream file(filePath);

  if (!file.good()) {
    LOG(ERROR) << "File handle not accesible for building polyline from file "
               << filePath;
    return false;
  }

  std::string word;

  // TODO msati3: Currently, just ensures consistency of numbering, more
  // elaborate schemes later?
  int startIndex = -1;
  size_t lastIndex = 0;
  size_t nextIndex;

  while (file >> word) {
    if (word == "v") {
      Kernel::Point_3 point;
      file >> point;
      polyline.add(point);
    } else if (word == "l") {
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
        } else {
          LOG(ERROR)
              << "Curve in obj file is not ordered (last, next) indices: "
              << lastIndex - 1 << "," << nextIndex;
          return false;
        }
      }
    }
  }
  return true;
}

// A vertex list file format consists of a sequence of vertices, each line
// consisting of delimited coordinates of a vertex. The polyline is assumed to
// be the line created by connecting consecutive vertices in the file.
bool buildPolylineFromVertexList(const std::string& filePath,
                                 GeometryPolyline_3& polyline) {
  std::ifstream file(filePath);

  if (!file.good()) {
    LOG(ERROR) << "File handle not accesible for building polyline from file "
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
        LOG(ERROR) << "Could not build polyline from file -- file not in "
                      "vertexlist format";
        return false;
      }
      pointCoords[index++] = std::stod(coord);
    }
    polyline.add(
        Kernel::Point_3(pointCoords[0], pointCoords[1], pointCoords[2]));
  }
  return true;
}
