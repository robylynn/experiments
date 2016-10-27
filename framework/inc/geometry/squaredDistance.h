#ifndef _FRAMEWORK_GEOMETRY_SQUARED_DISTANCE_H_
#define _FRAMEWORK_GEOMETRY_SQUARED_DISTANCE_H_

#include "polyline.h"
#include "polyloop.h"

// Add distance computation from points and rays for geometry types
namespace CGAL {

template <typename PointType>
Kernel::FT squared_distance(const Polyline<PointType>& polyline,
                            const Kernel::Point_3& point) {
  return polyline.squaredDistance(point);
}

template <typename PointType>
Kernel::FT squared_distance(const Kernel::Point_3& point,
                            const Polyline<PointType>& polyline) {
  return squared_distance(polyline, point);
}

template <typename PointType>
Kernel::FT squared_distance(const Polyline<PointType>& polyline,
                            const Kernel::Ray_3& ray) {
  return polyline.squaredDistance(ray);
}

template <typename PointType>
Kernel::FT squared_distance(const Kernel::Ray_3& ray,
                            const Polyline<PointType>& polyline) {
  return squared_distance(polyline, ray);
}

template <typename PointType>
Kernel::FT squared_distance(const Polyloop<PointType>& polyloop,
                            const Kernel::Point_3& point) {
  return polyloop.squaredDistance(point);
}

template <typename PointType>
Kernel::FT squared_distance(const Kernel::Point_3& point,
                            const Polyloop<PointType>& polyloop) {
  return squared_distance(polyloop, point);
}

template <typename PointType>
Kernel::FT squared_distance(const Polyloop<PointType>& polyloop,
                            const Kernel::Ray_3& ray) {
  return polyloop.squaredDistance(ray);
}

template <typename PointType>
Kernel::FT squared_distance(const Kernel::Ray_3& ray,
                            const Polyloop<PointType>& polyloop) {
  return squared_distance(polyloop, ray);
}

}  // end namespace CGAL

#endif //_FRAMEWORK_GEOMETRY_SQUARED_DISTANCE_H_
