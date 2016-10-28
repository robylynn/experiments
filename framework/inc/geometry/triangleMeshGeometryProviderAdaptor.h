#ifndef _FRAMEWORK_GEOMETRY_TRIANGLE_MESH_GEOMETRY_PROVIDER_ADAPTOR_H_
#define _FRAMEWORK_GEOMETRY_TRIANGLE_MESH_GEOMETRY_PROVIDER_ADAPTOR_H_

#include <type_traits>

#include <boost/iterator/counting_iterator.hpp>
#include <boost/iterator/filter_iterator.hpp>
#include <boost/iterator/transform_iterator.hpp>

#include <CGAL/Surface_mesh_complex_2_in_triangulation_3.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/Triangulation_2.h>

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
// interface for consumption by the geometry provider. Specialize this to adapt
// mesh geometry provider to provide renderable geometry for custom types.
template <typename MeshType>
class TriangleMeshGeometryProviderAdaptor {};

// Specialization that handles CGAL generated surface meshes that rely on a 3D
// triangulation algorithm. The facet for such results is a <tetrahedron cell,
// vertex index> pair. We need to iterate over all the vertices not equal to
// the vertex index.
template <typename Triangulation>
class TriangleMeshGeometryProviderAdaptor<
    CGAL::Surface_mesh_complex_2_in_triangulation_3<Triangulation>> {
  using MeshType =
      CGAL::Surface_mesh_complex_2_in_triangulation_3<Triangulation>;

 public:
  using facet_iterator = typename MeshType::Facet_iterator;
  using facet_vertex_iterator = boost::transform_iterator<
      std::function<const Kernel::Point_3&(int)>,
      boost::filter_iterator<std::function<int(int)>,
                             boost::counting_iterator<int>>>;
  using value_type = Kernel::Point_3;

  // Hold a facet, and, upon dereference, use the stored facet and the passed
  // in counting iterator index to look yield the vertex being referred to in
  // the triangulation data-structure.
  struct FacetHolderFunctor {
    FacetHolderFunctor(facet_iterator fIter) : facetIter(fIter) {}

    const Kernel::Point_3& operator()(int index) const {
      return facetIter->first->vertex(index)->point();
    }

    facet_iterator facetIter;
  };

  // For each tetrahedral cell, loop over the triangles that are in the 2D
  // complex from that facet (we do this by skipping the one index that the
  // facetIter points to)
  facet_vertex_iterator fv_iterator(facet_iterator facetIter) const {
    FacetHolderFunctor facetHolder(facetIter);
    std::function<int(int)> predicate = [skipIndex = facetIter->second](
        int vertexIndex) {
      return vertexIndex != skipIndex;
    };
    return boost::make_transform_iterator(
        boost::make_filter_iterator(
            predicate, boost::counting_iterator<int>(0),
            boost::counting_iterator<int>(FACETS_PER_TETRAHEDRON)),
        facetHolder);
  }

  const Kernel::Point_3& dereference(facet_vertex_iterator vertexIter) const {
    return *vertexIter;
  }

  facet_vertex_iterator next(facet_vertex_iterator vertexIter) const {
    return ++vertexIter;
  }

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
  using value_type = Kernel::Point_3;

  size_t size(const MeshType* mesh) const { return mesh->size_of_facets(); }

  facet_iterator facets_begin(const MeshType* mesh) const {
    return mesh->facets_begin();
  }

  facet_iterator facets_end(const MeshType* mesh) const {
    return mesh->facets_end();
  }
};

// Adaptor for all class that are a triangulation, providing a triangulation
// data structure
template <typename VB, typename FB>
class TriangleMeshGeometryProviderAdaptor<
    CGAL::Triangulation_data_structure_2<VB, FB>> {
  using MeshType = typename CGAL::Triangulation_data_structure_2<VB, FB>;

 public:
  using facet_iterator = typename MeshType::Face_iterator;
  using facet_vertex_iterator =
      boost::transform_iterator<std::function<const Kernel::Point_2&(int)>,
                                boost::counting_iterator<int>>;
  using value_type = Kernel::Point_2;

  // Hold a facet, and, upon dereference, use the stored facet and the passed
  // in counting iterator index to look yield the vertex being referred to in
  // the triangulation data-structure.
  struct FacetHolderFunctor {
    FacetHolderFunctor(facet_iterator fIter) : facetIter(fIter) {}

    const Kernel::Point_2& operator()(int index) const {
      return facetIter->vertex(index)->point();
    }

    facet_iterator facetIter;
  };

  // For each triangle face, loop over the vertices
  facet_vertex_iterator fv_iterator(facet_iterator facetIter) const {
    FacetHolderFunctor facetHolder(facetIter);
    return boost::make_transform_iterator(boost::counting_iterator<int>(0),
                                          facetHolder);
  }

  const Kernel::Point_2& dereference(facet_vertex_iterator vertexIter) const {
    return *vertexIter;
  }

  facet_vertex_iterator next(facet_vertex_iterator vertexIter) const {
    return ++vertexIter;
  }

  size_t size(const MeshType* mesh) const { return mesh->number_of_faces(); }

  facet_iterator facets_begin(const MeshType* mesh) const {
    return mesh->faces_begin();
  }

  facet_iterator facets_end(const MeshType* mesh) const {
    return mesh->faces_end();
  }
};

#endif  // _FRAMEWORK_GEOMETRY_TRIANGLE_MESH_GEOMETRY_PROVIDER_ADAPTOR_H_
