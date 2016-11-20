#ifndef _FRAMEWORK_GEOMETRY_GENERIC_VERTEX_ATTRIBUTES_ITERATORS_PROVIDER_H_
#define _FRAMEWORK_GEOMETRY_GENERIC_VERTEX_ATTRIBUTES_ITERATORS_PROVIDER_H_

#include "tupleElementIteratorAdaptor.h"
#include "attributes/vertexAttributesProviderTraits.h"

namespace impl {
/** Allows for GeometryReps that provide iteration over a container of
 * distinctly typed tuples of vertex attributes to provide iterators over
 * individual tuple types. A geometry rep can state to be primarily a vertex
 * attribute provider, in which case, it also has access to begin and end
 * iterators that forward to the vertices_begin calls. In case the geometry
 * representation holds a single vertex attributes, the begin and end calls
 * also unfold the tuple. Delayed instantiation ensures that the non-const
 * versions are not compiled into classes where they are not provided for by
 * the geometry rep.
 */
template <typename GeometryRep, typename VertexAttributes>
class GenericVertexAttributesIteratorsProvider {
 private:
  using VI = typename VertexAttributesProviderTraits<GeometryRep>::iterator;
  using CVI =
      typename VertexAttributesProviderTraits<GeometryRep>::const_iterator;
  using VB = std::remove_reference_t<decltype(*std::declval<VI>())>;

  template <typename Attrib>
  using Iter =
      decltype(utils::make_tuple_element_iterator<Attrib>(std::declval<VI>()));
  template <typename Attrib>
  using CIter =
      decltype(utils::make_tuple_element_iterator<Attrib>(std::declval<CVI>()));
  template <typename Attrib>
  using AT = utils::tag_to_type_t<Attrib>;

 public:
  // Iterators over VertexAttribute::type for a given vertex attribute.
  template <typename VertexAttrib>
  Iter<AT<VertexAttrib>> vertices_attrib_begin() {
    return utils::make_tuple_element_iterator<AT<VertexAttrib>>(
        static_cast<GeometryRep*>(this)->vertices_begin());
  }

  template <typename VertexAttrib>
  CIter<AT<VertexAttrib>> vertices_attrib_begin() const {
    return utils::make_tuple_element_iterator<const AT<VertexAttrib>>(
        static_cast<const GeometryRep*>(this)->vertices_begin());
  }

  template <typename VertexAttrib>
  Iter<AT<VertexAttrib>> vertices_attrib_end() {
    return utils::make_tuple_element_iterator<AT<VertexAttrib>>(
        static_cast<GeometryRep*>(this)->vertices_end());
  }

  template <typename VertexAttrib>
  CIter<AT<VertexAttrib>> vertices_attrib_end() const {
    return utils::make_tuple_element_iterator<const AT<VertexAttrib>>(
        static_cast<const GeometryRep*>(this)->vertices_end());
  }
};

}  // end of namespace impl

#endif  // _FRAMEWORK_GEOMETRY_GENERIC_VERTEX_ATTRIBUTES_ITERATORS_PROVIDER_H_
