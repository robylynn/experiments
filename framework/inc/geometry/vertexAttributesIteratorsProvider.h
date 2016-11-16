#ifndef _FRAMEWORK_GEOMETRY_VERTEX_ATTRIBUTES_ITERATORS_PROVIDER_H_
#define _FRAMEWORK_GEOMETRY_VERTEX_ATTRIBUTES_ITERATORS_PROVIDER_H_

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
  using VI = decltype(std::declval<GeometryRep>().vertices_begin());
  using CVI = decltype(std::declval<const GeometryRep>().vertices_begin());

 public:
  size_t size() { return this->vertices_size(); }
  VI begin() { return this->vertices_begin(); }
  CVI begin() const { return this->vertices_begin(); }
  VI end() { return this->vertices_end(); }
  CVI end() const { return this->vertices_end(); }
};

// If the primary simplex of a GeometryRep is a Vertex, also provide begin and
// end over unwrapped tuple type if the tuple contains a single element. This
// allows simple container like iteration of such providers.
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

 public:
  using iterator = VI;
  using const_iterator = CVI;

  size_t size() { return this->vertices_size(); }
  VI begin() { return this->template vertices_attrib_begin<TupleElement>(); }
  CVI begin() const {
    return this->template vertices_attrib_begin<TupleElement>();
  }
  VI end() { return this->template vertices_attrib_end<TupleElement>(); }
  CVI end() const { return this->template vertices_attrib_end<TupleElement>(); }
};

#endif  // _FRAMEWORK_GEOMETRY_VERTEX_ATTRIBUTES_ITERATORS_PROVIDER_H_
