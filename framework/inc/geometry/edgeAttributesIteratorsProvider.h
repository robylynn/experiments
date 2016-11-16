#ifndef _FRAMEWORK_GEOMETRY_EDGE_ATTRIBUTES_ITERATORS_PROVIDER_H_
#define _FRAMEWORK_GEOMETRY_EDGE_ATTRIBUTES_ITERATORS_PROVIDER_H_

#include "entityAttributeTraits.h"

// For arbitrary edge attributes, we need to store these in a vector. This is
// not currently implemented.
template <typename GeometryRep, bool primary_simplex = false,
          typename EdgeAttributes = std::tuple<Kernel::Segment_3>,
          typename SFINAE = void>
class EdgeAttributesIteratorsProvider {};

template <typename GR, bool PS = false,
          typename EAS = std::tuple<Kernel::Segment_3>, typename SFINAE = void>
using EdgeAttribsIters = EdgeAttributesIteratorsProvider<GR, PS, EAS, SFINAE>;

// For a simple edge position provider, an iterator over edge attributes (edge
// positions) can be generated on the fly, if the vertices iterator provides
// positions.
template <typename GeometryRep, bool primary_simplex>
class EdgeAttributesIteratorsProvider<
    GeometryRep, primary_simplex, std::tuple<Kernel::Segment_3>,
    typename std::enable_if<
        std::is_same<typename VertexAttributeTraits<GeometryRep>::value_type,
                     std::tuple<Kernel::Point_3>>::value>::type> {
  using VI = typename VertexAttributeTraits<GeometryRep>::iterator;
  using CVI = typename VertexAttributeTraits<GeometryRep>::const_iterator;

  class SegmentIterator
      : public boost::iterator_adaptor<
            SegmentIterator, CVI, std::tuple<Kernel::Segment_3>,
            boost::use_default, std::tuple<Kernel::Segment_3>> {
   public:
    explicit SegmentIterator(
        const GeometryRep* rep,
        const typename GeometryRep::const_vertex_iterator& baseIter)
        : boost::iterator_adaptor<SegmentIterator, CVI, const Kernel::Segment_3,
                                  boost::use_default, const Kernel::Segment_3>(
              baseIter),
          m_rep(rep) {}

   private:
    friend class boost::iterator_core_access;
    const std::tuple<Kernel::Segment_3> dereference() const {
      return std::make_tuple(m_rep->getSegment(this->base()));
    }
    const GeometryRep* m_rep;
  };

  using CEI = SegmentIterator;

 public:
  CEI edges_begin() const {
    return CEI(this, static_cast<const GeometryRep*>(this)->vertices_begin());
  }
  CEI edges_end() const {
    return CEI(this, static_cast<const GeometryRep*>(this)->vertices_end());
  }
};

#endif  // _FRAMEWORK_GEOMETRY_EDGE_ATTRIBUTES_ITERATORS_PROVIDER_H_
