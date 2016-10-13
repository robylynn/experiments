#ifndef _FRAMEWORK_GEOMETRY_MESHGEOMETRY_PROVIDER_H_
#define _FRAMEWORK_GEOMETRY_MESHGEOMETRY_PROVIDER_H_

#include <CGAL/SurfaceMeshComplex_2InTriangulation_3.h>

#include "geometryConstants.h"

class MeshTriangleListPolicy {
 public:
  static constexpr Ogre::RenderOperation::OperationType PRIMITIVE_TYPE =
      Ogre::RenderOperation::OT_TRIANGLES_LIST;
};

// The mesh geometry provider provides a stream of triangle vertices, one
// triangle at a time.
template <typename MeshType, typename ProviderPolicy = MeshTriangleListPolicy>
class MeshGeometryProvider : public ProviderPolicy {
 private:
  const MeshType* m_mesh;

 public:
  MeshGeometryProvider(const MeshType& mesh) : m_mesh(&mesh) {}
  MeshGeometryProvider(const MeshType&& mesh) = delete;
  ~MeshGeometryProvider() {}

  size_t size() const {
    return NUM_VERTICES_PER_TRIANGLE * m_mesh->number_of_facets();
  }

  class FacetOrderedVertexIterator
      : public boost::iterator_facade<VertexIterator, const Kernel::Point_3,
                                      boost::forward_traversal_tag> {
   public:
    FacetOrderedVertexIterator(const MeshGeometryProvider* provider,
                               const Facet_iterator facetIterator,
                               int vertexIndex)
        : m_provider(provider),
          m_facetIterator(facetIterator),
          m_vertexIndex(vertexIndex),
          m_halfEdge(facetIterator->halfedge()) {}

   private:
    friend class boost::iterator_core_access;

    void increment() {
      if (m_vertexIndex + 1 == VERTICES_PER_TRIANGLE) {
        m_vertexIndex = -1;
        if (++m_facetIterator == m_provider->facets_end()) {
          m_vertexIndex = VERTICES_PER_TRIANGLE - 1;
        }
      }
      m_halfEdge = m_halfEdge->next();
      ++m_vertexIndex;
    }

    bool equal(const FacetOrderedVertexIterator& other) {
      return (m_vertexIndex == other.m_vertexIndex) &&
             (m_facetIterator == other.m_facetIterator) &&
             (m_provider == other.m_provider);
    }

    const Kernel::Point_3& dereference() const {
      return m_halfEdge->vertex()->point();
    }

    MeshGeomtryProvider* m_provider;
    Facet_iterator m_facetIterator;
    size_t m_vertexIndex;
    Halfedge_handle m_halfEdge;
  };

  FacetOrderedVertexIterator begin() const {
    return FacetOrderedVertexIterator(this, m_mesh->facets_begin(), 0);
  }

  FacetOrderedVertexIterator end() const {
    return FacetOrderedVertexIterator(this, m_mesh->facets_end(),
                                      VERTICES_PER_TRIANGLE);
  }

  static constexpr int HINT_MAX_BOUND = 5000;
  using const_iterator = FacetOrderedVertexIterator;
};

#endif  //_FRAMEWORK_GEOMETRY_MESHGEOMETRY_PROVIDER_H_
