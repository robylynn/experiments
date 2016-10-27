#ifndef _FRAMEWORK_GEOMETRY_SQUARED_DISTANCE_H_
#define _FRAMEWORK_GEOMETRY_SQUARED_DISTANCE_H_

#include "polyline.h"
#include "polyloop_3.h"

// Add distance computation from points and rays for geometry types
namespace CGAL {

Kernel::FT squared_distance(const Polyline<Kernel::Point_3>& polyline,
                            const Kernel::Point_3& point) {
  return polyline.squaredDistance(point);
}

Kernel::FT squared_distance(const Kernel::Point_3& point,
                            const Polyline<Kernel::Point_3>& polyline) {
  return squared_distance(polyline, point);
}

Kernel::FT squared_distance(const Polyline<Kernel::Point_3>& polyline,
                            const Kernel::Ray_3& ray) {
  return polyline.squaredDistance(ray);
}

Kernel::FT squared_distance(const Kernel::Ray_3& ray,
                            const Polyline<Kernel::Point_3>& polyline) {
  return squared_distance(polyline, ray);
}

Kernel::FT squared_distance(const Polyloop_3& polyloop,
                            const Kernel::Point_3& point) {
  return polyloop.squaredDistance(point);
}

Kernel::FT squared_distance(const Kernel::Point_3& point,
                            const Polyloop_3& polyloop) {
  return squared_distance(polyloop, point);
}

Kernel::FT squared_distance(const Polyloop_3& polyloop,
                            const Kernel::Ray_3& ray) {
  return polyloop.squaredDistance(ray);
}

Kernel::FT squared_distance(const Kernel::Ray_3& ray,
                            const Polyloop_3& polyloop) {
  return squared_distance(polyloop, ray);
}

}  // end namespace CGAL

#endif //_FRAMEWORK_GEOMETRY_SQUARED_DISTANCE_H_
