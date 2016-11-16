#ifndef _FRAMEWORK_GEOMETRY_GENERIC_VERTEX_ATTRIBUTES_ITERATORS_PROVIDER_H_
#define _FRAMEWORK_GEOMETRY_GENERIC_VERTEX_ATTRIBUTES_ITERATORS_PROVIDER_H_

#include "tupleElementIteratorAdaptor.h"
#include "entityAttributeTraits.h"

namespace impl {
// Allows for GeometryReps that provide iteration over a container of
// distinctly typed tuples of vertex attributes to provide iterators over
// individual tuple types. A geometry rep can state to be primarily a vertex
// attribute provider, in which case, it also has access to begin and end
// iterators that forward to the vertices_begin calls. In case the geometry
// representation holds a single vertex attributes, the begin and end calls
// also unfold the tuple.
template <typename GeometryRep, typename VertexAttributes>
class GenericVertexAttributesIteratorsProvider {
 private:
  using VI = typename VertexAttributeTraits<GeometryRep>::iterator;
  using CVI = typename VertexAttributeTraits<GeometryRep>::const_iterator;
  using VB = std::remove_reference_t<decltype(*std::declval<VI>())>;

  template <typename VA>
  using Iter =
      decltype(utils::make_tuple_element_iterator<VA>(std::declval<VI>()));
  template <typename VA>
  using CIter =
      decltype(utils::make_tuple_element_iterator<VA>(std::declval<CVI>()));

 public:
  // Iterators over vertex attributes.
  template <typename VertexAttrib>
  Iter<VertexAttrib> vertices_attrib_begin() {
    return utils::make_tuple_element_iterator<VertexAttrib>(
        static_cast<GeometryRep*>(this)->vertices_begin());
  }

  template <typename VertexAttrib>
  CIter<VertexAttrib> vertices_attrib_begin() const {
    return utils::make_tuple_element_iterator<const VertexAttrib>(
        static_cast<const GeometryRep*>(this)->vertices_begin());
  }

  template <typename VertexAttrib>
  Iter<VertexAttrib> vertices_attrib_end() {
    return utils::make_tuple_element_iterator<VertexAttrib>(
        static_cast<GeometryRep*>(this)->vertices_end());
  }

  template <typename VertexAttrib>
  CIter<VertexAttrib> vertices_attrib_end() const {
    return utils::make_tuple_element_iterator<const VertexAttrib>(
        static_cast<const GeometryRep*>(this)->vertices_end());
  }
};

}  // end of namespace impl

#endif  // _FRAMEWORK_GEOMETRY_GENERIC_VERTEX_ATTRIBUTES_ITERATORS_PROVIDER_H_
