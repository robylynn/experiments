#ifndef _GEOMETRY_VARIANTS_H_
#define _GEOMETRY_VARIANTS_H_

#include <boost/variant.hpp>

#include "polyloop_3.h"

// The geometry types that support distance computation from a point
using PointDistanceComputableTypes =
    boost::variant<Kernel::Point_3, Kernel::Line_3, Kernel::Ray_3,
                   Kernel::Segment_3, Kernel::Plane_3,
                   Polyloop_3>;

#endif  //_GEOMETRY_VARIANTS_H_
