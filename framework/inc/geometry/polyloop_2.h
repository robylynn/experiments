#ifndef _FRAMEWORK_GEOMETRY_POLYLOOP2_H_
#define _FRAMEWORK_GEOMETRY_POLYLOOP2_H_

#include <CGAL/Polygon_2.h>
#include <CGAL/Segment_2.h>

#include "geometryTypes.h"

#include <containerAlgorithms.h>

// A polyloop_2 is a closed polyline in 2D. Representation wise, a polyloop_2
// simply adapts a CGAL::Polygon_2 object. The generic code is not implemented
// as yet for polyloops.
//
// TODO msati3: Look at making generic.
class Polyloop_2 {
  using Polygon_2 = CGAL::Polygon_2<Kernel>;
  using PointType = Kernel::Point_2;
  using PointsContainer = Polygon_2;
  PointsContainer m_points;

 public:
  using SegmentIterator = Polygon_2::Edge_const_iterator;
  using value_type = PointType;
  using iterator = typename PointsContainer::Vertex_iterator;
  using const_iterator = typename PointsContainer::Vertex_const_iterator;

  // A hint on the maximum size that a polyloop may have. This may be useful
  // for rendering in non-immediate mode.
  static constexpr size_t HINT_MAX_BOUND = 10000;

  // The size of a Polyloop is the number of points it contains
  size_t size() const { return m_points.size(); }

  // Add a point at specified location to the Polyloop.
  void addPoint(iterator iter, const PointType& point) {
    m_points.insert(iter, point);
  }

  // Add a point after the last point. If no point in the Polyloop, add
  // point as first point.
  void addPoint(const PointType& point) {
    m_points.insert(m_points.vertices_end(), point);
  }

  // Append a bunch of points to the end of the Polyloop.
  template <typename PointsIterator>
  void addPoints(const PointsIterator& begin, const PointsIterator& end) {
    for (PointsIterator iter = begin; iter != end; ++iter) {
      addPoint(*iter);
    }
  }

  void updatePoint(typename PointsContainer::iterator iter,
                   const PointType& point) {
    m_points.set(iter, point);
  }

  // The polyloop allows for iteration over points by pass through to container,
  // and conforms to the CGAL MeshPolyline_3 concept
  auto begin() const -> decltype(m_points.vertices_begin()) {
    return m_points.vertices_begin();
  }
  auto end() const -> decltype(m_points.vertices_end()) {
    return m_points.vertices_end();
  }

  SegmentIterator beginSegment() const {
    return m_points.edges_begin();
  }
  SegmentIterator endSegment() const { return m_points.edges_end(); }

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
    const Kernel::Segment_2& closestSegment =
        *(std::min_element(beginSegment(), endSegment(),
                           [&other](const Kernel::Segment_2& first,
                                    const Kernel::Segment_2& second) {
                             return CGAL::squared_distance(other, first) <
                                    CGAL::squared_distance(other, second);
                           }));
    return CGAL::squared_distance(other, closestSegment);
  }

  // Obtain segment corresponding to the current iterator (there is a
  // one-to-one mapping between the two)
  Kernel::Segment_2 getSegment(const const_iterator& iterator) const {
    return Kernel::Segment_2(*iterator, *next(iterator));
  }
};

// Build a polyloop from Obj file format
bool buildPolyloop_2FromObj(const std::string& filePath, Polyloop_2& polyloop);

#endif  //_FRAMEWORK_GEOMETRY_POLYLOOP2_H_
