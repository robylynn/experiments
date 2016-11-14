#ifndef _FRAMEWORK_GEOMETRY_POLYLOOP_3_H_
#define _FRAMEWORK_GEOMETRY_POLYLOOP_3_H_

#include <boost/iterator/iterator_adaptor.hpp>

#include <vector>

#include <CGAL/circulator.h>
#include <CGAL/squared_distance_3.h>

#include <containerAlgorithms.h>

#include "geometryTypes.h"

// For arbitrary edge attributes, we need to store these in a vector. This is
// not currently implemented.
template <typename Polyloop, typename EdgeBase>
class EdgeAttributeProvider {};

// For a simple edge position provider, an iterator over edge attributes (edge
// positions) can be generated on the fly.
template <typename Polyloop>
class EdgeAttributeProvider<Polyloop, std::tuple<Kernel::Segment_3>> {
  class SegmentIterator
      : public boost::iterator_adaptor<
            SegmentIterator, typename Polyloop::const_vertex_iterator,
            std::tuple<Kernel::Segment_3>, boost::use_default,
            std::tuple<Kernel::Segment_3>> {
   public:
    explicit SegmentIterator(
        const Polyloop* loop,
        const typename Polyloop::const_vertex_iterator& baseIter)
        : boost::iterator_adaptor<SegmentIterator,
                                  typename Polyloop::const_vertex_iterator,
                                  const Kernel::Segment_3, boost::use_default,
                                  const Kernel::Segment_3>(baseIter),
          m_loop(loop) {}

   private:
    friend class boost::iterator_core_access;
    const std::tuple<Kernel::Segment_3> dereference() const {
      return std::make_tuple(m_loop->getSegment(this->base()));
    }
    const Polyloop* m_loop;
  };

 public:
  using const_edge_iterator = SegmentIterator;
  const_edge_iterator edges_begin() const {
    return const_edge_iterator(this, this->vertices_begin());
  }

  const_edge_iterator edges_end() const {
    return const_edge_iterator(this, this->vertices_end());
  }
};

template <typename GeometryRep>
class VertexAttributeTraits {};

// Allows for GeometryReps that provide iteration over a container of
// distinctly typed tuples of vertex attributes to provide iterators over
// individual tuple types.
//
// The GeometryReps must provide vertex_iterator and const_vertex_iterator
// typedef, and vertices_begin and vertices_end functions for iterating over
// these.
template <typename GeometryRep,
          typename VertexAttributes =
              typename VertexAttributeTraits<GeometryRep>::vertex_value_type>
class GenericVertexAttributeProvider {
 private:
  using VI = typename VertexAttributeTraits<GeometryRep>::vertex_iterator;
  using CVI =
      typename VertexAttributeTraits<GeometryRep>::const_vertex_iterator;
  using VB = typename VertexAttributeTraits<GeometryRep>::vertex_value_type;

  template <typename VertexAttrib>
  using Iter = boost::transform_iterator<std::function<VertexAttrib&(VB&)>, VI>;
  template <typename VertexAttrib>
  using CIter =
      boost::transform_iterator<std::function<const VertexAttrib&(const VB&)>,
                                CVI>;

 public:
  // Iterators over vertex attributes.
  template <typename VertexAttrib>
  Iter<VertexAttrib> vertices_attrib_begin() {
    return Iter<VertexAttrib>(static_cast<GeometryRep*>(this)->vertices_begin(),
                              utils::tupleset_typed_index<VertexAttrib, VB>);
  }

  template <typename VertexAttrib>
  CIter<VertexAttrib> vertices_attrib_begin() const {
    return CIter<VertexAttrib>(
        static_cast<GeometryRep*>(this)->vertices_begin(),
        utils::tupleset_typed_index<VertexAttrib, VB>);
  }

  template <typename VertexAttrib>
  Iter<VertexAttrib> vertices_attrib_end() {
    return Iter<VertexAttrib>(static_cast<GeometryRep*>(this)->vertices_end(),
                              utils::tupleset_typed_index<VertexAttrib, VB>);
  }

  template <typename VertexAttrib>
  CIter<VertexAttrib> vertices_attrib_end() const {
    return CIter<VertexAttrib>(static_cast<GeometryRep*>(this)->vertices_end(),
                               utils::tupleset_typed_index<VertexAttrib, VB>);
  }
};

template <typename GeometryRep, typename VertexAttributes,
          bool primary_simplex = false>
class VertexAttributeProvider
    : public GenericVertexAttributeProvider<GeometryRep, VertexAttributes> {};

// If the primary simplex of a GeometryRep is a Vertex, provide begin and end
// iterators over tuple entries.
template <typename GeometryRep, typename VertexAttributes>
class VertexAttributeProvider<GeometryRep, VertexAttributes, true>
    : public GenericVertexAttributeProvider<GeometryRep, VertexAttributes> {
 private:
  using VI = typename VertexAttributeTraits<GeometryRep>::vertex_iterator;
  using CVI =
      typename VertexAttributeTraits<GeometryRep>::const_vertex_iterator;
  using VB = typename VertexAttributeTraits<GeometryRep>::vertex_value_type;

 public:
  VI info_begin() { return this->vertices_begin(); }
  CVI info_begin() const { return this->vertices_begin(); }
  VI info_end() { return this->vertices_end(); }
  CVI info_end() const { return this->vertices_end(); }
};

// If the primary simplex of a GeometryRep is a Vertex, also provide begin and
// end over unwrapped tuple type if the tuple contains a single element. This
// allows simple container like iteration of such providers.
template <typename GeometryRep, typename TupleElement>
class VertexAttributeProvider<GeometryRep, std::tuple<TupleElement>, true>
    : public GenericVertexAttributeProvider<GeometryRep,
                                            std::tuple<TupleElement>> {
 public:
  auto begin()
      -> std::result_of<this->vertices_attrib_begin<TupleElement>>::type {
    return this->vertices_attrib_begin<TupleElement>();
  }
  auto begin() const
      -> std::result_of<this->vertices_attrib_begin<TupleElement>>::type {
    return this->vertices_attrib_begin<TupleElement>();
  }
  auto end() -> std::result_of<this->vertices_attrib_end<TupleElement>>::type {
    return this->vertices_attrib_end<TupleElement>();
  }

  auto end() const
      -> std::result_of<this->vertices_attrib_end<TupleElement>>::type {
    return this->vertices_attrib_end<TupleElement>();
  }
};

// A polyloop is a closed polyline.
// Representation wise, a polyloop is representable using a container of
// vertices, with adjacent vertices implicitly understood to be
// connected to
// each other. The vertices themselves carry a tuple of (discrete type)
// vertex
// attributes. Each of the vertex attribute types need to be
// CopyConstructible
// and assignable.

// The polyloop also exposes a const iterator for its composing
// LineSegments.  If the polyloop has no edge specific information, an
// iterator
// over edge positions can simply generated on the fly, and this
// information
// need not be stored. Thus, this policy decision of providing
// EdgeAttributes
// is designated to a policy class, which must provide for iteration
// over
// EdgeBase elements.
template <typename VertexBase = std::tuple<Kernel::Point_3>,
          typename EdgeBase = std::tuple<Kernel::Segment_3>>
class GPolyloop_3
    : public EdgeAttributeProvider<GPolyloop_3<VertexBase, EdgeBase>, EdgeBase>,
      public VertexAttributeProvider<GPolyloop_3<VertexBase, EdgeBase>> {
  using VertexAttributeContainer = typename VertexAttributeTraits<
      Polyloop_3<VertexBase, EdgeBase>>::container_type;
  VertexAttributeContainer m_vertexAttribs;

 public:
  using vertex_value_type = typename VertexAttributeContainer::value_type;
  using vertex_iterator = typename VertexAttributeContainer::iterator;
  using const_vertex_iterator =
      typename VertexAttributeContainer::const_iterator;
  using iterator = vertex_iterator;
  using const_iterator = vertex_iterator;

  Polyloop_3(){};
  Polyloop_3(const VertexAttributeContainer&& attribs)
      : m_vertexAttribs(std::move(attribs)) {}

  // A hint on the maximum size that a polyloop may have. This may be useful
  // for rendering in non-immediate mode.
  static constexpr size_t HINT_MAX_BOUND = 10000;

  // The size of a Polyloop is the number of vertices it contains.
  size_t size() const { return m_vertexAttribs.size(); }

  // Add a vertex attribute to the Polyloop.
  void add(vertex_iterator iter, const vertex_value_type& value) {
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

  // The polyloop allows for iteration over vertices by pass through to
  // container, and conforms to the CGAL MeshPolyline_3 concept.
  vertex_iterator vertices_begin() { return m_vertexAttribs.begin(); }
  const_vertex_iterator vertices_begin() const {
    return m_vertexAttribs.begin();
  }

  vertex_iterator vertices_end() { return m_vertexAttribs.end(); }
  const_vertex_iterator vertices_end() const { return m_vertexAttribs.end(); }

  // Obtain next iterators from the current iterators.
  vertex_iterator next_vertex(const vertex_iterator& iterator) {
    return iterator + 1 == vertices_end() ? vertices_begin() : iterator + 1;
  }

  const_vertex_iterator next_vertex(
      const const_vertex_iterator& iterator) const {
    return iterator + 1 == vertices_end() ? vertices_begin() : iterator + 1;
  }

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
  }

 private:
  // Obtain segment corresponding to the current iterator (there is a
  // one-to-one mapping between the two).
  Kernel::Segment_3 getSegment(const const_vertex_iterator& iterator) const {
    return Kernel::Segment_3(*iterator, *next_vertex(iterator));
  }
};

template <typename VertexBase, typename EdgeBase>
struct VertexAttributeTraits<Polyloop_3<VertexBase, EdgeBase>> {
 private:
  using PT = Polyloop_3<VertexBase, EdgeBase>;

 public:
  using vertex_value_type = VertexBase;
  using container_type = std::vector<vertex_value_type>;
  using vertex_iterator = typename container_type::iterator;
  using const_vertex_iterator = typename container_type::const_iterator;
};

// A simple geometry polyloop also adds begin and end iterators over the single
// type they contain
template <typename TupleElement, typename EdgeBase>
class SimplePolyloop_3 : public Polyloop_3<> {};

// Build a polyloop from different file formats.
bool buildPolyloopFromObj(const std::string& filePath,
                          GeometryPolyloop_3& polyloop);
bool buildPolyloopFromVertexList(const std::string& filePath,
                                 GeometryPolyloop_3& polyloop);

#endif  //_FRAMEWORK_GEOMETRY_POLYLOOP_3_H_
