#include <CGAL/bounding_box.h>

#include "polyloop_2.h"
#include "polyloop_3.h"

bool buildPolyloopFromObj(const std::string& filePath, Polyloop_2& polyloop2) {
  Polyloop_3 polyloop3;
  bool retValue = buildPolyloopFromObj(filePath, polyloop3);
  if (!retValue) return retValue;

  Kernel::Iso_cuboid_3 cuboid =
      CGAL::bounding_box(polyloop3.begin(), polyloop3.end());
  Kernel::Vector_3 vect = cuboid.max() - cuboid.min();
  float axisToDrop = abs(vect.x()) < abs(vect.y()) ? 0 : 1;
  axisToDrop =
      abs(vect.z()) < std::min(abs(vect.x()), abs(vect.y())) ? 2 : axisToDrop;
  std::set<int> coordinateAxes = {0, 1, 2};
  coordinateAxes.erase(axisToDrop);

  for (const Kernel::Point_3& point : polyloop3) {
    polyloop2.addPoint(Kernel::Point_2(point[*coordinateAxes.begin()],
                                       point[*(++coordinateAxes.begin())]));
  }
  return true;
}
