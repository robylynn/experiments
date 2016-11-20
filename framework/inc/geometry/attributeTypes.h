#ifndef _FRAMEWORK_GEOMETRY_ATTRIBUTE_TYPES_H_
#define _FRAMEWORK_GEOMETRY_ATTRIBUTE_TYPES_H_

#include <CGAL/Point_3.h>
#include <CGAL/IO/Color.h>

#include "geometryTypes.h"
#include "attributes/attributesToTypes.h"

/* A collection of common attributes that are associated with geometry
 * representations. The tagging of attributes allows multiple attributes to
 * have the same underlying type, but still a different semantic meaning (a
 * normal attribute and a binormal attribute, for example.
 */
struct PositionAttribute_3 {
  using type = Kernel::Point_3;
};

struct ColorAttribute {
  using type = CGAL::Color;
};

struct SegmentAttribute_3 {
  using type = Kernel::Segment_3;
};

#endif//_FRAMEWORK_GEOMETRY_ATTRIBUTE_TYPES_H_
