#ifndef _FRAMEWORK_GEOMETRY_SQUARED_DISTANCE_H_
#define _FRAMEWORK_GEOMETRY_SQUARED_DISTANCE_H_

#include "polyline_3.h"
#include "polyloop_3.h"
#include "polyloop_2.h"

// Add distance computation from points and rays for geometry types
namespace CGAL {

inline Kernel::FT squared_distance(const Polyline<Kernel::Point_3>& polyline,
                            const Kernel::Point_3& point) {
  return polyline.squaredDistance(point);
}

inline Kernel::FT squared_distance(const Kernel::Point_3& point,
                            const Polyline_3& polyline) {
  return squared_distance(polyline, point);
}

inline Kernel::FT squared_distance(const Polyline_3& polyline,
                            const Kernel::Ray_3& ray) {
  return polyline.squaredDistance(ray);
}

inline Kernel::FT squared_distance(const Kernel::Ray_3& ray,
                            const Polyline_3& polyline) {
  return squared_distance(polyline, ray);
}

inline Kernel::FT squared_distance(const Polyloop_3& polyloop,
                            const Kernel::Point_3& point) {
  return polyloop.squaredDistance(point);
}

inline Kernel::FT squared_distance(const Kernel::Point_3& point,
                            const Polyloop_3& polyloop) {
  return squared_distance(polyloop, point);
}

inline Kernel::FT squared_distance(const Polyloop_3& polyloop,
                            const Kernel::Ray_3& ray) {
  return polyloop.squaredDistance(ray);
}

inline Kernel::FT squared_distance(const Kernel::Ray_3& ray,
                            const Polyloop_3& polyloop) {
  return squared_distance(polyloop, ray);
}

inline Kernel::FT squared_distance(const Polyloop_2& polyloop,
                            const Kernel::Point_2& point) {
  return polyloop.squaredDistance(point);
}

inline Kernel::FT squared_distance(const Kernel::Point_2& point,
                            const Polyloop_2& polyloop) {
  return squared_distance(polyloop, point);
}

inline Kernel::FT squared_distance(const Polyloop_2& polyloop,
                            const Kernel::Ray_2& ray) {
  return polyloop.squaredDistance(ray);
}

inline Kernel::FT squared_distance(const Kernel::Ray_2& ray,
                            const Polyloop_2& polyloop) {
  return squared_distance(polyloop, ray);
}

}  // end namespace CGAL

#endif  //_FRAMEWORK_GEOMETRY_SQUARED_DISTANCE_H_
