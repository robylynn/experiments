#ifndef _FRAMEWORK_GEOMETRY_POLYLOOP_3_H_
#define _FRAMEWORK_GEOMETRY_POLYLOOP_3_H_

#include <boost/iterator/iterator_adaptor.hpp>

#include <vector>

#include <CGAL/circulator.h>
#include <CGAL/squared_distance_3.h>

#include <containerAlgorithms.h>

#include "geometryTypes.h"
#include "vertexAttributesIteratorsProvider.h"
#include "edgeAttributesIteratorsProvider.h"

// A polyloop is a closed polyline.
// Representation wise, a polyloop is
// representable using a container of vertices, with adjacent vertices
// implicitly understood to be connected to each other. The vertices themselves
// carry a tuple of (discrete type) vertex attributes. Each of the vertex
// attribute types need to be CopyConstructible and assignable.

// The polyloop also exposes a const iterator for its composing edges.
// If the polyloop has no edge specific information, an iterator over edge
// positions can simply generated on the fly, and this information need not be
// stored. Thus, this policy decision of providing EdgeAttributes is designated
// to a policy class, which must provide for iteration over EdgeBase elements.
template <typename VertexBase = std::tuple<Kernel::Point_3>,
          typename EdgeBase = std::tuple<Kernel::Segment_3>>
class Polyloop_3 : public EdgeAttribsIters<Polyloop_3<VertexBase, EdgeBase>,
                                           false, EdgeBase>,
                   public VertexAttribsIters<Polyloop_3<VertexBase, EdgeBase>,
                                             true, VertexBase> {
  using VertexAttributesContainer = typename VertexAttributeTraits<
      Polyloop_3<VertexBase, EdgeBase>>::container_type;
  VertexAttributesContainer m_vertexAttribs;

 public:
  using vertex_value_type = typename VertexAttributesContainer::value_type;
  using vertex_iterator = typename VertexAttributesContainer::iterator;
  using const_vertex_iterator =
      typename VertexAttributesContainer::const_iterator;

  Polyloop_3(){};
  Polyloop_3(const VertexAttributesContainer&& attribs)
      : m_vertexAttribs(std::move(attribs)) {}

  // Add a vertex attribute to the Polyloop.
  void add(const vertex_iterator& iter, const vertex_value_type& value) {
    m_vertexAttribs.insert(iter, value);
  }

  // Add an attribute after the last location. If no attributes in the Polyloop,
  // add at the first location.
  void add(const vertex_value_type& value) { m_vertexAttribs.push_back(value); }

  vertex_iterator remove(vertex_iterator iter) {
    return m_vertexAttribs.erase(iter);
  }

  void update(vertex_iterator iter, const vertex_value_type& value) {
    *iter = value;
  }

  // Update just a particular vertex attribute, not the set of all vertex
  // attributes.
  template <typename AttributeType>
  void update(vertex_iterator iter, const AttributeType& attributeValue) {
    std::get<AttributeType>(*iter) = attributeValue;
  }

  size_t vertices_size() const { return m_vertexAttribs.size(); }

  // The polyloop allows for iteration over vertices by pass through to
  // container, and conforms to the CGAL MeshPolyline_3 concept.
  vertex_iterator vertices_begin() { return m_vertexAttribs.begin(); }
  const_vertex_iterator vertices_begin() const {
    return m_vertexAttribs.begin();
  }

  vertex_iterator vertices_end() { return m_vertexAttribs.end(); }
  const_vertex_iterator vertices_end() const { return m_vertexAttribs.end(); }

  // Obtain next iterators from the current iterators.
  template <typename Iter>
  Iter next_vertex(const Iter& iterator) {
    return iterator + 1 == Iter(vertices_end()) ? Iter(vertices_begin())
                                                : iterator + 1;
  }

  template <typename Iter>
  Iter next_vertex(const Iter& iterator) const {
    return iterator + 1 == Iter(vertices_end()) ? Iter(vertices_begin())
                                                : iterator + 1;
  }

  /*
  // Algorithms on Polyloops
  // Given a query for OtherRep, implement distance as the minimum over
  // constituent line segments.
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
  }*/
};

template <typename VertexBase, typename EdgeBase>
struct VertexAttributeTraits<Polyloop_3<VertexBase, EdgeBase>> {
 public:
  using value_type = VertexBase;
  template <typename Value = value_type>
  using container_type = std::vector<value_type>;
  using iterator = typename container_type::iterator;
  using const_iterator = typename container_type::const_iterator;
};

// A simple geometry polyloop also adds begin and end iterators over the single
// type they contain
template <typename TupleElement>
using SimplePolyloop_3 = Polyloop_3<std::tuple<TupleElement>>;
using GeometryPolyloop_3 = SimplePolyloop_3<Kernel::Point_3>;

// Build a polyloop from different file formats.
bool buildPolyloop_3FromObj(const std::string& filePath,
                            GeometryPolyloop_3& polyloop);
bool buildPolyloop_3FromVertexList(const std::string& filePath,
                                   GeometryPolyloop_3& polyloop);

#endif  //_FRAMEWORK_GEOMETRY_POLYLOOP_3_H_
