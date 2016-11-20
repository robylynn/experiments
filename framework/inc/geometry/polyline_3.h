#ifndef _FRAMEWORK_GEOMETRY_POLYLINE_3_H_
#define _FRAMEWORK_GEOMETRY_POLYLINE_3_H_

#include <boost/iterator/iterator_adaptor.hpp>

#include <vector>

#include <CGAL/circulator.h>
#include <CGAL/squared_distance_3.h>

#include "geometryTypes.h"
#include "attributeTypes.h"
#include "attributes/vertexAttributesIteratorsProvider.h"
#include "attributes/edgeAttributesIteratorsProvider.h"

// A polyline is an ordered sequence of vertices.
// Representation wise, a polyline is representable using a container of vertex
// attributes, with adjacent vertices implicitly understood to be connected to
// each other.
//
// The polyline also exposes a const iterator for its composing edges.
// If the polyline holds positions as attributes, and has no edge specific
// information, an iterator over edge positions can simply generated on the fly,
// and this information need not be stored. Thus, this policy decision
// of providing EdgeAttributes is designated to a policy class, which must
// provide for iteration over EdgeBase elements.
template <typename VertexBase = std::tuple<PositionAttribute_3>,
          typename EdgeBase = std::tuple<SegmentAttribute_3>>
class Polyline_3
    : public EdgeAttribsIters<Polyline_3<VertexBase, EdgeBase>, false,
                              attributes_to_types_t<EdgeBase>>,
      public VertexAttribsIters<Polyline_3<VertexBase, EdgeBase>, true,
                                attributes_to_types_t<VertexBase>> {
  using VertexAttributesContainer = typename VertexAttributeTraits<
      Polyline_3<VertexBase, EdgeBase>>::container_type;
  VertexAttributesContainer m_vertexAttribs;

 public:
  using vertex_value_type = typename VertexAttributesContainer::value_type;
  using vertex_iterator = typename VertexAttributesContainer::iterator;
  using const_vertex_iterator =
      typename VertexAttributesContainer::const_iterator;

  // The size of a Polyline is the number of points it contains
  size_t size() const { return m_vertexAttribs.size(); }

  // Add a vertex at specified location to the Polyline.
  void add(const vertex_iterator& iter, const vertex_value_type& value) {
    m_vertexAttribs.insert(iter, value);
  }

  // Add a vertex after the last point. If no vertex in the Polyline, add
  // vertex as first point.
  void add(const vertex_value_type& value) { m_vertexAttribs.push_back(value); }

  vertex_iterator remove(const vertex_iterator& iter) {
    return m_vertexAttribs.erase(iter);
  }

  void update(vertex_iterator iter, const vertex_value_type& value) {
    m_vertexAttribs.assign(iter, iter + 1, value);
  }

  // Update just a particular vertex attribute, not the set of all vertex
  // attributes.
  template <typename AttributeType>
  void update(vertex_iterator iter, const AttributeType& attributeValue) {
    std::get<AttributeType>(*iter) = attributeValue;
  }

  size_t vertices_size() const { return m_vertexAttribs.size(); }

  // The polyline allows for iteration over points by pass through to container,
  // and conforms to the CGAL MeshPolyline_3 concept
  auto begin() -> decltype(m_vertexAttribs.begin()) {
    return m_vertexAttribs.begin();
  }
  auto begin() const -> decltype(m_vertexAttribs.begin()) {
    return m_vertexAttribs.begin();
  }
  auto end() -> decltype(m_vertexAttribs.end()) {
    return m_vertexAttribs.end();
  }
  auto end() const -> decltype(m_vertexAttribs.end()) {
    return m_vertexAttribs.end();
  }

  // Algorithms on Polylines
  // Given a query for OtherRep, implement distance as the minimum over
  // constituent line segments
  template <typename OtherRep>
  FieldType squaredDistance(const OtherRep& other) const {
    const Kernel::Segment_3& closestSegment =
        *(std::min_element(this->edges_begin(), this->edges_end(),
                           [&other](const Kernel::Segment_3& first,
                                    const Kernel::Segment_3& second) {
                             return CGAL::squared_distance(other, first) <
                                    CGAL::squared_distance(other, second);
                           }));
    return CGAL::squared_distance(other, closestSegment);
  }
};

template <typename VertexBase, typename EdgeBase>
struct VertexAttributeTraits<Polyline_3<VertexBase, EdgeBase>> {
 public:
  using value_type = attributes_to_types_t<VertexBase>;
  using container_type = std::vector<value_type>;
  using iterator = typename container_type::iterator;
  using const_iterator = typename container_type::const_iterator;
};

// A simple geometry polyloop also adds begin and end iterators over the single
// type they contain
template <typename Attribute>
using SimplePolyline_3 = Polyline_3<std::tuple<Attribute>>;
using GeometryPolyline_3 = SimplePolyline_3<PositionAttribute_3>;

// Build a polyline from Obj file format
bool buildPolyline_3FromObj(const std::string& filePath,
                            GeometryPolyline_3& polyline);
bool buildPolyline_3FromVertexList(const std::string& filePath,
                                   GeometryPolyline_3& polyline);

#endif  //_FRAMWORK_GEOMETRY_POLYLINE_3_H_
