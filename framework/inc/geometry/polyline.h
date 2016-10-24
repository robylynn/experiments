#ifndef _POLYLINE_H_
#define _POLYLINE_H_

#include <boost/iterator/iterator_adaptor.hpp>

#include <vector>

#include <CGAL/circulator.h>
#include <CGAL/squared_distance_3.h>

#include "geometryTypes.h"

// A polyline is an ordered sequence of polylines.
// Representation wise, a polyline is representable using a container of points,
// with adjacent points implicitly understood to be connected to each other.

// The polyline also exposes a const iterator for its composing LineSegments
template <typename PointType = Kernel::Point_3>
class Polyline {
  using PointsContainer = std::vector<PointType>;

  PointsContainer m_points;

 public:
  using value_type = PointType;
  using iterator = typename PointsContainer::iterator;
  using const_iterator = typename PointsContainer::const_iterator;

  // A hint on the maximum size that a polyline may have. This may be useful
  // for rendering in non-immediate mode.
  static constexpr size_t HINT_MAX_BOUND = 10000;

  Polyline() {}

  // The size of a Polyline is the number of points it contains
  size_t size() const { return m_points.size(); }

  // Add a point at specified location to the Polyline.
  void addPoint(iterator iter, const PointType& point) {
    m_points.insert(iter, point);
  }

  // Add a point after the last point. If no point in the Polyline, add
  // point as first point.
  void addPoint(const PointType& point) { m_points.push_back(point); }

  iterator removePoint(iterator iter) { return m_points.erase(iter); }

  void updatePoint(typename PointsContainer::iterator iter,
                   const PointType& point) {
    m_points.assign(iter, iter + 1, point);
  }

  // The polyline allows for iteration over points by pass through to container,
  // and conforms to the CGAL MeshPolyline_3 concept
  auto begin() -> decltype(m_points.begin()) { return m_points.begin(); }
  auto begin() const -> decltype(m_points.begin()) { return m_points.begin(); }
  auto end() -> decltype(m_points.end()) { return m_points.end(); }
  auto end() const -> decltype(m_points.end()) { return m_points.end(); }

  // The polyline also allows for iteration over its constituting Segments.
  // This is done using the segment iterator. This only provides for constant
  // iteration.
  class SegmentIterator
      : public boost::iterator_adaptor<SegmentIterator, const_iterator,
                                       Kernel::Segment_3, boost::use_default,
                                       Kernel::Segment_3> {
   public:
    // Note: Red herring -- the SegmentIterator doesn't cache the created
    // Kernel::Segment_3 object anywhere. Thus, the type of the returned
    // reference must be Kernel::Segment_3, as, otherwise, consumers would be
    // storing a reference to a temporary that is already destructed when
    // operator* exits.
    explicit SegmentIterator(const Polyline<PointType>* line,
                             const const_iterator& baseIter)
        : boost::iterator_adaptor<SegmentIterator, const_iterator,
                                  Kernel::Segment_3, boost::use_default,
                                  Kernel::Segment_3>(baseIter),
          m_line(line) {}

   private:
    friend class boost::iterator_core_access;
    Kernel::Segment_3 dereference() const {
      return m_line->getSegment(this->base());
    }
    const Polyline<PointType>* m_line;
  };

  SegmentIterator beginSegment() const {
    return SegmentIterator(this, begin());
  }
  SegmentIterator endSegment() const {
    return SegmentIterator(this, end() - 1);
  }

  // Obtain next iterators from the current iterators
  iterator next(const iterator& iterator) { return iterator + 1; }

  const_iterator next(const const_iterator& iterator) const {
    return iterator + 1;
  }

  // Algorithms on Polylines
  // Given a query point, find the squared distance from the polyline
  FieldType squaredDistance(const Kernel::Point_3& point) const {
    const Kernel::Segment_3& closestSegment =
        *(std::min_element(beginSegment(), endSegment(),
                           [&point](const Kernel::Segment_3& first,
                                    const Kernel::Segment_3& second) {
                             return CGAL::squared_distance(point, first) <
                                    CGAL::squared_distance(point, second);
                           }));
    return CGAL::squared_distance(point, closestSegment);
  }

 private:
  // Obtain segment corresponding to the current iterator (there is a
  // one-to-one mapping between the two)
  Kernel::Segment_3 getSegment(const const_iterator& iterator) const {
    return Kernel::Segment_3(*iterator, *next(iterator));
  }
};

// Build a polyline from Obj file format
template <typename PointType>
bool buildPolylineFromObj(const std::string& filePath,
                          Polyline<PointType>& polyline);

// Add distance computation from point to CGAL
namespace CGAL {

template <typename PointType>
Kernel::FT squared_distance(const Polyline<PointType>& polyline,
                            const Kernel::Point_3& point) {
  return polyline.squaredDistance(point);
}

}  // namespace CGAL

#endif  //_POLYLINE_H_