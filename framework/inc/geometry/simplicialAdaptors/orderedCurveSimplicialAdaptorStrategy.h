#ifndef _FRAMEWORK_GEOMETRY_ORDERED_CURVE_SIMPLICIAL_ADAPTOR_STRATEGY_H_
#define _FRAMEWORK_GEOMETRY_ORDERED_CURVE_SIMPLICIAL_ADAPTOR_STRATEGY_H_

#include "simplexTypes.h"

// A adaptor strategy for any geometry representation that provides and ordered
// container of points. Iteration over such a collection of points will provide
// a pointlist, while stencil iteration using a {0,1} stencil will provide a
// line list.

// Ordered curves can all work off the following adaptor strategy, to obtain
// stencil iterators that yield the simplicial decomposition from vertex
// iterators.
template <typename SimplexType>
class OrderedCurveSimplicialAdaptorStrategy {};

template <>
class OrderedCurveSimplicialAdaptorStrategy<PointList> {
 public:
  static constexpr size_t VERTICES_PER_BASE = 1;
  static const std::array<int, VERTICES_PER_BASE>& stencil() {
    static constexpr std::array<int, VERTICES_PER_BASE> stencil = {{0}};
    return stencil;
  }
};

template <>
class OrderedCurveSimplicialAdaptorStrategy<LineList> {
 public:
  static constexpr size_t VERTICES_PER_BASE = 2;
  static const std::array<int, VERTICES_PER_BASE>& stencil() {
    static constexpr std::array<int, VERTICES_PER_BASE> stencil = {{0, 1}};
    return stencil;
  }
};

#endif  // _FRAMEWORK_GEOMETRY_ORDERED_CURVE_SIMPLICIAL_ADAPTOR_STRATEGY_H_
