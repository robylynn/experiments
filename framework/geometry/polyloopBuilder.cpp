#include <fstream>

#include <glog/logging.h>

#include <CGAL/Point_3.h>
#include "polyloop.h"

// Creates a polyloop from an obj file
template <>
bool buildPolyloopFromObj<Kernel::Point_3>(
    const std::string& filePath, Polyloop<Kernel::Point_3>& polyloop) {
  std::ifstream file(filePath);

  if (!file.good()) {
    LOG(ERROR) << "File handle not accesible for building polyloop from file "
               << filePath;
    return false;
  }

  bool fClosedLoop = false;
  std::string word;

  // TODO msati3: Currently, just ensures consistency of numbering, more
  // elaborate schemes later?
  constexpr size_t START_INDEX = 1;
  size_t lastIndex = START_INDEX;
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
      if (nextIndex != lastIndex) {
        LOG(ERROR) << "Curve in obj file skips an index (last, next) indices: "
                   << lastIndex << "," << nextIndex;
        return false;
      }
      file >> nextIndex;
      if (nextIndex != ++lastIndex) {
        if (nextIndex == START_INDEX) {
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

  DLOG_IF(ERROR, !fClosedLoop) << "Polyloop is not closed";
  return fClosedLoop;
}
