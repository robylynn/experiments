#include <fstream>

#include <glog/logging.h>

#include "polyline.h"

// Creates a polyline from an obj file
template <>
bool buildPolylineFromObj<Kernel::Point_3>(
    const std::string& filePath, Polyline<Kernel::Point_3>& polyline) {
  std::ifstream file(filePath);

  if (!file.good()) {
    LOG(ERROR) << "File handle not accesible for building polyline from file "
               << filePath;
    return false;
  }

  std::string word;

  // TODO msati3: Currently, just ensures consistency of numbering, more
  // elaborate schemes later?
  constexpr size_t START_INDEX = 1;
  size_t lastIndex = START_INDEX;
  size_t nextIndex;

  while (file >> word) {
    if (word == "v") {
      Kernel::Point_3 point;
      file >> point;
      polyline.addPoint(point);
    } else if (word == "l") {
      file >> nextIndex;
      if (nextIndex != lastIndex) {
        LOG(ERROR) << "Curve in obj file skips an index (last, next) indices: "
                   << lastIndex << "," << nextIndex;
        return false;
      }
      file >> nextIndex;
      if (nextIndex != ++lastIndex) {
        if (nextIndex == START_INDEX) {
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
