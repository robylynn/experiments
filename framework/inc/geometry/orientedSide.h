#ifndef _FRAMEWORK_GEOMETRY_ORIENTED_SIDE_H_
#define _FRAMEWORK_GEOMETRY_ORIENTED_SIDE_H_

#include <CGAL/Polygon_2_algorithms.h>

#include "polyloop_2.h"

// Add distance computation from points and rays for geometry types
namespace CGAL {

CGAL::Oriented_side oriented_side(const Polyloop_2& polyloop,
                                  const Kernel::Point_2& point) {
  return CGAL::oriented_side_2(polyloop.begin(), polyloop.end(), point,
                               Kernel());
}

}  // end namespace CGAL

#endif //_FRAMEWORK_GEOMETRY_ORIENTED_SIDE_H_
