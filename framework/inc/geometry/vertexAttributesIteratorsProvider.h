#ifndef _FRAMEWORK_GEOMETRY_VERTEX_ATTRIBUTES_ITERATORS_PROVIDER_H_
#define _FRAMEWORK_GEOMETRY_VERTEX_ATTRIBUTES_ITERATORS_PROVIDER_H_

#include <boost/iterator/indirect_iterator.hpp>

#include "impl/genericVertexAttributesIteratorsProvider.h"

template <typename GeometryRep, bool primary_simplex = false,
          typename VertexAttributes = std::remove_reference_t<decltype(
              *std::declval<GeometryRep>().vertices_begin())>>
class VertexAttributesIteratorsProvider
    : public impl::GenericVertexAttributesIteratorsProvider<GeometryRep,
                                                            VertexAttributes> {
};

template <typename GR, bool PS = false,
          typename VAS = std::remove_reference_t<decltype(
              *std::declval<GR>().vertices_begin())>>
using VertexAttribsIters = VertexAttributesIteratorsProvider<GR, PS, VAS>;

// If the primary simplex of a GeometryRep is a Vertex, provide begin and end
// iterators over tuple entries.
template <typename GeometryRep, typename VertexAttributes>
class VertexAttributesIteratorsProvider<GeometryRep, true, VertexAttributes>
    : public impl::GenericVertexAttributesIteratorsProvider<GeometryRep,
                                                            VertexAttributes> {
 private:
  using VI = typename VertexAttributeTraits<GeometryRep>::iterator;
  using CVI = typename VertexAttributeTraits<GeometryRep>::const_iterator;

  GeometryRep& rep() { return *static_cast<GeometryRep*>(this); }
  const GeometryRep& rep() const {
    return *static_cast<const GeometryRep*>(this);
  }

 public:
  size_t size() const { return rep().vertices_size(); }
  VI begin() { return rep().vertices_begin(); }
  CVI begin() const { return rep().vertices_begin(); }
  VI end() { return rep().vertices_end(); }
  CVI end() const { return rep().vertices_end(); }
};

// If the primary simplex of a GeometryRep is a Vertex, also provide begin and
// end over container. However, we also add transparent dereferencing to
// unfolded tuple element in this case.
template <typename GeometryRep, typename TupleElement>
class VertexAttributesIteratorsProvider<GeometryRep, true,
                                        std::tuple<TupleElement>>
    : public impl::GenericVertexAttributesIteratorsProvider<
          GeometryRep, std::tuple<TupleElement>> {
  using Base =
      impl::GenericVertexAttributesIteratorsProvider<GeometryRep,
                                                     std::tuple<TupleElement>>;

  using VI = decltype(
      std::declval<Base>().template vertices_attrib_begin<TupleElement>());
  using CVI = decltype(std::declval<const Base>()
                           .template vertices_attrib_begin<TupleElement>());

  Base& rep() { return *static_cast<Base*>(this); }
  const Base& rep() const { return *static_cast<const Base*>(this); }

 public:
  // using iterator = decltype(std::declval<GeometryRep>().begin());
  // using const_iterator = decltype(std::declval<const GeometryRep>().begin());

  size_t size() const {
    return static_cast<const GeometryRep*>(this)->vertices_size();
  }
  VI begin() { return rep().template vertices_attrib_begin<TupleElement>(); }
  CVI begin() const {
    return rep().template vertices_attrib_begin<TupleElement>();
  }
  VI end() { return rep().template vertices_attrib_end<TupleElement>(); }
  CVI end() const { return rep().template vertices_attrib_end<TupleElement>(); }
};

#endif  // _FRAMEWORK_GEOMETRY_VERTEX_ATTRIBUTES_ITERATORS_PROVIDER_H_
