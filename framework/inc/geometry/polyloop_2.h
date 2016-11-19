#ifndef _FRAMEWORK_GEOMETRY_POLYLOOP2_H_
#define _FRAMEWORK_GEOMETRY_POLYLOOP2_H_

#include <CGAL/Polygon_2.h>
#include <CGAL/Segment_2.h>

#include "geometryTypes.h"

#include <containerAlgorithms.h>

#include "attributes/vertexAttributesIteratorsProvider.h"
#include "attributes/edgeAttributesIteratorsProvider.h"

// A polyloop_2 is a closed polyline in 2D. Representation wise, a polyloop_2
// simply adapts a CGAL::Polygon_2 object. The generic code is not implemented
// as yet for polyloops.
//
// TODO msati3: Look at making generic.
class Polyloop_2 {
  using Polygon_2 = CGAL::Polygon_2<Kernel>;
  using vertex_value_type = typename Polygon_2::value_type;
  using VertexAttributesContainer = Polygon_2;
  VertexAttributesContainer m_vertexAttribs;

 public:
  using SegmentIterator = Polygon_2::Edge_const_iterator;
  using value_type = vertex_value_type;
  using iterator = Polygon_2::Vertex_iterator;
  using const_iterator = Polygon_2::Vertex_const_iterator;

  // The size of a Polyloop is the number of points it contains
  size_t size() const { return m_vertexAttribs.size(); }

  // Add a vertex at specified location to the Polyloop.
  void add(iterator iter, const vertex_value_type& point) {
    m_vertexAttribs.insert(iter, point);
  }

  // Add a vertex after the last point. If no vertex in the Polyloop, add
  // vetex as first vertex.
  void add(const vertex_value_type& point) {
    m_vertexAttribs.insert(m_vertexAttribs.vertices_end(), point);
  }

  // Append a bunch of vertices to the end of the Polyloop.
  template <typename Iterator>
  void add(const Iterator& begin, const Iterator& end) {
    for (Iterator iter = begin; iter != end; ++iter) {
      add(*iter);
    }
  }

  void update(typename VertexAttributesContainer::iterator iter,
              const vertex_value_type& value) {
    m_vertexAttribs.set(iter, value);
  }

  // The polyloop allows for iteration over points by pass through to container,
  // and conforms to the CGAL MeshPolyline_3 concept
  auto begin() const -> decltype(m_vertexAttribs.vertices_begin()) {
    return m_vertexAttribs.vertices_begin();
  }
  auto end() const -> decltype(m_vertexAttribs.vertices_end()) {
    return m_vertexAttribs.vertices_end();
  }

  SegmentIterator beginSegment() const { return m_vertexAttribs.edges_begin(); }
  SegmentIterator endSegment() const { return m_vertexAttribs.edges_end(); }

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

template <>
struct VertexAttributeTraits<Polyloop_2> {
 public:
  using value_type = Kernel::Point_2;
  using container_type = CGAL::Polygon_2<Kernel>;
  using iterator = typename container_type::Vertex_iterator;
  using const_iterator = typename container_type::Vertex_const_iterator;
};

using GeometryPolyloop_2 = Polyloop_2;

// Build a polyloop from Obj file format
bool buildPolyloop_2FromObj(const std::string& filePath, Polyloop_2& polyloop);

#endif  //_FRAMEWORK_GEOMETRY_POLYLOOP2_H_
