#ifndef _FRAMEWORK_GEOMETRY_ATTRIBUTES_H_
#define _FRAMEWORK_GEOMETRY_ATTRIBUTES_H_

#include <CGAL/Point_3.h>
#include <CGAL/IO/Color.h>

#include "geometryTypes.h"

// A collection of common attributes that are associated with geometry
// representations. The tagging of attributes allows multiple attributes to
// have the same underlying type, but still a different semantic meaning (a
// normal attribute and a binormal attribute, for example.
struct PositionAttribute_3 {
  using type = Kernel::Point_3;
};

struct ColorAttribute {
  using type = CGAL::Color;
};

#endif//_FRAMEWORK_GEOMETRY_ATTRIBUTES_H_
