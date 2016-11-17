#ifndef _FRAMEWORK_GEOMETRY_TRIANGLE_MESH_SIMPLICIAL_ADAPTOR_H_
#define _FRAMEWORK_GEOMETRY_TRIANGLE_MESH_SIMPLICIAL_ADAPTOR_H_

#include <OGRE/OgreRenderOperation.h>

#include <CGAL/Triangulation_3.h>

#include "geometryTypes.h"
#include "geometryConstants.h"
#include "triangleMeshSimplicialProviderAPIAdaptor.h"
#include "simplexTypes.h"

// The triangle mesh geometry provider provides a stream of triangle vertices,
// one triangle at a time from a triangle mesh representation.
//
// Now, triangle meshes may have different representations. The TriangleMeshAPI
// adaptor serves to unite these differences and present a unified interface to
// the TriangleMeshSimplicialAdaptor.
template <typename MeshType, typename SimplexType = TriangleList>
class TriangleMeshSimplicialAdaptor
    : public TriangleMeshRepAPIAdaptor<MeshType, SimplexType> {
 private:
  using MeshAPIAdaptor = TriangleMeshAPIAdaptor<MeshType, SimplexType>;
  const MeshType* m_mesh;
  MeshAPIAdaptor m_adaptor;

 public:
  TriangleMeshGeometryProvider(const MeshType& mesh) : m_mesh(&mesh) {}
  TriangleMeshGeometryProvider(const MeshType&& mesh) = delete;
  ~TriangleMeshGeometryProvider() {}

  class FacetOrderedVertexIterator
      : public boost::iterator_facade<FacetOrderedVertexIterator,
                                      const typename MeshAPIAdaptor::value_type,
                                      boost::forward_traversal_tag> {
   public:
    FacetOrderedVertexIterator(
        const MeshAPIAdaptor* adaptor,
        typename MeshAPIAdaptor::facet_iterator facetIter, int vertexIndex)
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

    const typename MeshAPIAdaptor::value_type& dereference() const {
      return m_adaptor->dereference(m_facetVertexIter);
    }

    const MeshAPIAdaptor* m_adaptor;
    typename MeshAPIAdaptor::facet_iterator m_facetIter;
    typename MeshAPIAdaptor::facet_vertex_iterator m_facetVertexIter;
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

  using const_iterator = FacetOrderedVertexIterator;
};

// A triangle mesh geometry provider is a lightweight object. So, we specialize
// the storage policy to by value. This allows for nicer client syntax through
// implicit temporary creation for MeshGeometryProvider.
template <typename ElementProvider, typename ElementType>
class ElementProviderStorageStrategy<
    TriangleMeshGeometryProvider<ElementProvider, ElementType>>
    : public CopyProviderStorageStrategy<
          TriangleMeshGeometryProvider<ElementProvider, ElementType>> {
 protected:
  using CopyProviderStorageStrategy<TriangleMeshGeometryProvider<
      ElementProvider, ElementType>>::CopyProviderStorageStrategy;
};

#endif  //_FRAMEWORK_GEOMETRY_TRIANGLE_MESH_SIMPLICIAL_ADAPTOR_H_
