#ifndef _POLYLOOP_3_H_
#define _POLYLOOP_3_H_

#include <boost/iterator/iterator_adaptor.hpp>

#include <vector>

#include <CGAL/circulator.h>
#include <CGAL/squared_distance_3.h>

#include "geometryTypes.h"

// A polyloop is a closed polyline.
// Representation wise, a polyloop is representable using a container of points,
// with adjacent points implicitly understood to be connected to each other.

// The polyloop also exposes a const iterator for its composing LineSegments
class Polyloop_3 {
  using PointsContainer = std::vector<Kernel::Point_3>;
  PointsContainer m_points;

 public:
  using value_type = Kernel::Point_3;
  using iterator = typename PointsContainer::iterator;
  using const_iterator = typename PointsContainer::const_iterator;

  // A hint on the maximum size that a polyloop may have. This may be useful
  // for rendering in non-immediate mode.
  static constexpr size_t HINT_MAX_BOUND = 10000;

  Polyloop_3() {}

  // The size of a Polyloop is the number of points it contains
  size_t size() const { return m_points.size(); }

  // Add a point at specified location to the Polyloop.
  void addPoint(iterator iter, const value_type& point) {
    m_points.insert(iter, point);
  }

  // Add a point after the last point. If no point in the Polyloop, add
  // point as first point.
  void addPoint(const value_type& point) { m_points.push_back(point); }

  iterator removePoint(iterator iter) { return m_points.erase(iter); }

  void updatePoint(iterator iter, const value_type& point) { *iter = point; }

  // The polyloop allows for iteration over points by pass through to container,
  // and conforms to the CGAL MeshPolyline_3 concept
  iterator begin() { return m_points.begin(); }
  const_iterator begin() const { return m_points.begin(); }
  iterator end() { return m_points.end(); }
  const_iterator end() const { return m_points.end(); }

  // The polyloop also allows for iteration over its constituting Segments.
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
    explicit SegmentIterator(const Polyloop_3* loop,
                             const const_iterator& baseIter)
        : boost::iterator_adaptor<SegmentIterator, const_iterator,
                                  Kernel::Segment_3, boost::use_default,
                                  Kernel::Segment_3>(baseIter),
          m_loop(loop) {}

   private:
    friend class boost::iterator_core_access;
    Kernel::Segment_3 dereference() const {
      return m_loop->getSegment(this->base());
    }
    const Polyloop_3* m_loop;
  };

  SegmentIterator beginSegment() const {
    return SegmentIterator(this, begin());
  }
  SegmentIterator endSegment() const { return SegmentIterator(this, end()); }

  // Obtain next iterators from the current iterators
  iterator next(const iterator& iterator) {
    return iterator + 1 == end() ? begin() : iterator + 1;
  }

  const_iterator next(const const_iterator& iterator) const {
    return iterator + 1 == end() ? begin() : iterator + 1;
  }

  // Algorithms on Polyloops
  // Given a query for OtherRep, implement distance as the minimum over
  // constituent line segments
  template <typename OtherRep>
  FieldType squaredDistance(const OtherRep& other) const {
    const Kernel::Segment_3& closestSegment =
        *(std::min_element(beginSegment(), endSegment(),
                           [&other](const Kernel::Segment_3& first,
                                    const Kernel::Segment_3& second) {
                             return CGAL::squared_distance(other, first) <
                                    CGAL::squared_distance(other, second);
                           }));
    return CGAL::squared_distance(other, closestSegment);
  }

 private:
  // Obtain segment corresponding to the current iterator (there is a
  // one-to-one mapping between the two)
  Kernel::Segment_3 getSegment(const const_iterator& iterator) const {
    return Kernel::Segment_3(*iterator, *next(iterator));
  }
};

// Build a polyloop from Obj file format
bool buildPolyloopFromObj(const std::string& filePath, Polyloop_3& polyloop);

#endif  //_POLYLOOP_3_H_
