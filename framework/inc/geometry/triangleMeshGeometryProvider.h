#ifndef _FRAMEWORK_GEOMETRY_TRIANGLEMESHGEOMETRY_PROVIDER_H_
#define _FRAMEWORK_GEOMETRY_TRIANGLEMESHGEOMETRY_PROVIDER_H_

#include <OGRE/OgreRenderOperation.h>

#include <CGAL/Triangulation_3.h>

#include "geometryTypes.h"
// TODO msati3: Use circulator for facet vertex iteration, and get rid of this
// include
#include "geometryConstants.h"
#include "triangleMeshGeometryProviderAdaptor.h"

class MeshTriangleListPolicy {
 public:
  static constexpr Ogre::RenderOperation::OperationType PRIMITIVE_TYPE =
      Ogre::RenderOperation::OT_TRIANGLE_LIST;
};

// The triangle mesh geometry provider provides a stream of triangle vertices,
// one triangle at a time from a triangle mesh representation
template <typename MeshType, typename ProviderPolicy = MeshTriangleListPolicy>
class TriangleMeshGeometryProvider : public ProviderPolicy {
 private:
  using GeometryProvider =
      TriangleMeshGeometryProvider<MeshType, ProviderPolicy>;
  using MeshAdaptor = TriangleMeshGeometryProviderAdaptor<MeshType>;
  const MeshType* m_mesh;
  MeshAdaptor m_adaptor;

 public:
  TriangleMeshGeometryProvider(const MeshType& mesh) : m_mesh(&mesh) {}
  TriangleMeshGeometryProvider(const MeshType&& mesh) = delete;
  ~TriangleMeshGeometryProvider() {}

  class FacetOrderedVertexIterator
      : public boost::iterator_facade<
            FacetOrderedVertexIterator,
            const typename MeshAdaptor::value_type,
            boost::forward_traversal_tag> {
   public:
    FacetOrderedVertexIterator(const MeshAdaptor* adaptor,
                               typename MeshAdaptor::facet_iterator facetIter,
                               int vertexIndex)
        : m_adaptor(adaptor),
          m_facetIter(facetIter),
          m_vertexIndex(vertexIndex),
          m_facetVertexIter(m_adaptor->fv_iterator(facetIter)) {}

   private:
    friend class boost::iterator_core_access;

    void increment() {
      m_facetVertexIter = m_adaptor->next(m_facetVertexIter);
      if (++m_vertexIndex == VERTICES_PER_TRIANGLE) {
        m_vertexIndex = 0;
        ++m_facetIter;
        m_facetVertexIter = m_adaptor->fv_iterator(m_facetIter);
      }
    }

    bool equal(const FacetOrderedVertexIterator& other) const {
      return (m_vertexIndex == other.m_vertexIndex) &&
             (m_facetIter == other.m_facetIter) &&
             (m_adaptor == other.m_adaptor);
    }

    const typename MeshAdaptor::value_type& dereference() const {
      return m_adaptor->dereference(m_facetVertexIter);
    }

    const MeshAdaptor* m_adaptor;
    typename MeshAdaptor::facet_iterator m_facetIter;
    typename MeshAdaptor::facet_vertex_iterator m_facetVertexIter;
    size_t m_vertexIndex;
  };

  size_t size() const { return VERTICES_PER_TRIANGLE * m_adaptor.size(m_mesh); }

  FacetOrderedVertexIterator begin() const {
    return FacetOrderedVertexIterator(&m_adaptor,
                                      m_adaptor.facets_begin(m_mesh), 0);
  }

  FacetOrderedVertexIterator end() const {
    return FacetOrderedVertexIterator(&m_adaptor, m_adaptor.facets_end(m_mesh),
                                      0);
  }

  static constexpr int HINT_MAX_BOUND = 100000;
  using const_iterator = FacetOrderedVertexIterator;
};

#endif  //_FRAMEWORK_GEOMETRY_TRIANGLEMESHGEOMETRY_PROVIDER_H_
