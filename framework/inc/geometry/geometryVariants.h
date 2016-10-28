#ifndef _GEOMETRY_VARIANTS_H_
#define _GEOMETRY_VARIANTS_H_

#include <boost/variant.hpp>

#include "geometryTypes.h"
#include "polyloop_3.h"
#include "polyloop_2.h"

// The geometry types that support distance computation from a point
template <typename Domain>
class PointDistanceComputableTypes {};

template <>
class PointDistanceComputableTypes<Kernel::Point_3> {
 public:
  using type = boost::variant<Kernel::Point_3, Kernel::Line_3, Kernel::Ray_3,
                              Kernel::Segment_3, Kernel::Plane_3, Polyloop_3>;
};

template <>
class PointDistanceComputableTypes<Kernel::Point_2> {
 public:
  using type = boost::variant<Kernel::Point_2, Kernel::Line_2, Kernel::Ray_2,
                              Kernel::Segment_2, Polyloop_2>;
};

template <typename Domain>
class SignedDistanceComputableTypes {};

template <>
class SignedDistanceComputableTypes<Kernel::Point_2> {
 public:
  using type = boost::variant<Kernel::Line_2, Polyloop_2>;
};

#endif  //_GEOMETRY_VARIANTS_H_
