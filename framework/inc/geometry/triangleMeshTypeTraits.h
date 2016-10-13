#ifndef _FRAMEWORK_GEOMETRY_TRIANGLE_MESH_GEOMETRY_PROVIDER_ADAPTOR_H_
#define _FRAMEWORK_GEOMETRY_TRIANGLE_MESH_GEOMETRY_PROVIDER_ADAPTOR_H_

#include <CGAL/Polyhedron_3.h>

// If the MeshRepresentation is based on HalfEdge data-structure, we can reuse
// some of the code.
template <typename T, typename FI, typename FVI>
class HalfEdgeBasedGeometryProviderAdaptor {
 public:
  using facet_iterator = FI;
  using facet_vertex_iterator = FVI;

  facet_vertex_iterator fv_iterator(facet_iterator facetIter) const {
    return facetIter->halfedge();
  }

  const Kernel::Point_3& dereference(facet_vertex_iterator vertexIter) const {
    return vertexIter->vertex()->point();
  }

  facet_vertex_iterator next(facet_vertex_iterator vertexIter) const {
    return vertexIter->next();
  }
};

// Code to adapt some common triangle mesh containers of CGAL to use a uniform
// interface for consumption by the geometry provider. The default match
// assumes a half edge based representation.
template <typename MeshType>
class TriangleMeshGeometryProviderAdaptor
    : public HalfEdgeBasedGeometryProviderAdaptor<
          TriangleMeshGeometryProviderAdaptor<MeshType>,
          typename MeshType::Facet_iterator,
          typename MeshType::Vertex_handle> {
 public:
  using facet_iterator = typename MeshType::Facet_iterator;
  using facet_vertex_iterator = typename MeshType::Vertex_handle;

  size_t size(const MeshType* mesh) const { return mesh->number_of_facets(); }

  facet_iterator facets_begin(const MeshType* mesh) const {
    return mesh->facets_begin();
  }

  facet_iterator facets_end(const MeshType* mesh) const {
    return mesh->facets_end();
  }
};

template <typename Kernel>
class TriangleMeshGeometryProviderAdaptor<CGAL::Polyhedron_3<Kernel>>
    : public HalfEdgeBasedGeometryProviderAdaptor<
          TriangleMeshGeometryProviderAdaptor<CGAL::Polyhedron_3<Kernel>>,
          typename CGAL::Polyhedron_3<Kernel>::Facet_const_iterator,
          typename CGAL::Polyhedron_3<Kernel>::Halfedge_const_handle> {
  using MeshType = CGAL::Polyhedron_3<Kernel>;

 public:
  using facet_iterator =
      typename CGAL::Polyhedron_3<Kernel>::Facet_const_iterator;
  using facet_vertex_iterator =
      typename CGAL::Polyhedron_3<Kernel>::Halfedge_const_handle;

  size_t size(const MeshType* mesh) const { return mesh->size_of_facets(); }

  facet_iterator facets_begin(const MeshType* mesh) const {
    return mesh->facets_begin();
  }

  facet_iterator facets_end(const MeshType* mesh) const {
    return mesh->facets_end();
  }
};

#endif  // _FRAMEWORK_GEOMETRY_TRIANGLE_MESH_GEOMETRY_PROVIDER_ADAPTOR_H_
