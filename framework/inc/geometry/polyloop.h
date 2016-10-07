#ifndef _POLYLOOP_H_
#define _POLYLOOP_H_

#include <boost/iterator/transform_iterator.hpp>

#include <vector>
#include <CGAL/circulator.h>

#include "geometryTypes.h"

// A polyloop is an ordered sequence of polylines that form a closed loop.
// Representation wise, a polyloop is representable using a container of points,
// with adjacent points implicitly understood to be connected to each other.

// The polyloop also exposes a const iterator for its composing LineSegments
// TODO msati3: Expose this interface
template <typename PointType>
class Polyloop {
  using PointsContainer = std::vector<PointType>;

  PointsContainer m_points;

 public:
  using value_type = PointType;
  using iterator = typename PointsContainer::iterator;
  using const_iterator = typename PointsContainer::const_iterator;

  // A hint on the maximum size that a polyloop may have. This may be useful
  // for rendering in non-immediate mode.
  static constexpr size_t HINT_MAX_BOUND = 1000;

  Polyloop() {}

  // The size of a Polyloop is the number of points it contains
  size_t size() const { return m_points.size(); }

  // Add a point at specified location to the Polyloop.
  void addPoint(iterator iter, const PointType& point) {
    m_points.insert(iter, point);
  }

  // Add a point after the last point. If no point in the Polyloop, add
  // point as first point.
  void addPoint(const PointType& point) { m_points.push_back(point); }

  iterator removePoint(iterator iter) { return m_points.erase(iter); }

  void updatePoint(typename PointsContainer::iterator iter,
                   const PointType& point) {
    m_points.assign(iter, iter + 1, point);
  }

  // The polyloop allows for iteration over points by pass through to container,
  // and conforms to the CGAL MeshPolyline_3 concept
  auto begin() -> decltype(m_points.begin()) { return m_points.begin(); }
  auto begin() const -> decltype(m_points.begin()) { return m_points.begin(); }
  auto end() -> decltype(m_points.end()) { return m_points.end(); }
  auto end() const -> decltype(m_points.end()) { return m_points.end(); }

  // Obtain next iterators from the current iterators
  iterator next(const iterator& iterator) {
    return iterator + 1 == end() ? begin() : iterator + 1;
  }

  const_iterator next(const const_iterator& iterator) const {
    return iterator + 1 == end() ? begin() : iterator + 1;
  }

  // Obtain segment corresponding to the current iterator (there is a
  // one-to-one mapping between the two)
  Kernel::Segment_3 getSegment(const const_iterator& iterator) const {
    return Kernel::Segment_3(*iterator, *next(iterator));
  }

  /*
  // Algorithms on Polyloops
  // Given a query point, find the distance of t
  FieldType computeDistance(const Kernel::Point_3& point) const {
    using SegmentCreator =
        std::function<Kernel::Segment_3(const const_iterator&)>;
    using SegmentIterator =
        boost::transform_iterator<SegmentCreator, const const_iterator,
                                  Kernel::Segment_3>;

    SegmentCreator segmentCreationFunction =
        std::bind(&Polyloop::getSegment, this, std::placeholders::_1);
    SegmentIterator segmentIterator =
        boost::make_transform_iterator(begin(), segmentCreationFunction);
    SegmentIterator closestSegment = std::min_element(
        segmentIterator.begin(), segmentIterator.end(),
        [&point](const SegmentIterator& first, const SegmentIterator& second) {
          return CGAL::squared_distance(point, first) <
                 CGAL::squared_distance(point, second);
        });
    return CGAL::squared_distance(point, closestSegment);
  }*/
};

// Add distance computation from point to CGAL
namespace CGAL {

template <typename PointType>
Kernel::FT squared_distance(const Polyloop<PointType>& polyloop,
                            const Kernel::Point_3 &point) {
  return 0;
}

}  // namespace CGAL

#endif  //_POLYLOOP_H_
