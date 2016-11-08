#ifndef _FRAMEWORK_RENDERING_DEFAULT_ELEMENT_TYPES_H_
#define _FRAMEWORK_RENDERING_DEFAULT_ELEMENT_TYPES_H_

#include <CGAL/IO/Color.h>
#include "vertexElement.h"

// A number of default mappings from simple primitive types (CGAL/geom), to the
// type of VertexElementProvider a container of such types is.
// Among other things, this allows simple STL containers to provide vertex
// element with automatically deduced provision types.
template <typename T>
struct VertexElementFromType {};

// A vector provides different vertex elements, dependant on its contained type
template <typename T>
struct VertexElementFromType<typename std::vector<T>> {
  using type = typename VertexElementFromType<T>::type;
};

template <typename Kernel>
struct VertexElementFromType<typename CGAL::Point_3<Kernel>> {
  using type = PositionVertexElement;
};

template <typename Kernel>
struct VertexElementFromType<typename CGAL::Point_2<Kernel>> {
  using type = PositionVertexElement;
};

template <>
struct VertexElementFromType<CGAL::Color> {
  using type = ColorVertexElement;
};

#endif  //_FRAMEWORK_RENDERING_DEFAULT_ELEMENT_TYPES_H_
