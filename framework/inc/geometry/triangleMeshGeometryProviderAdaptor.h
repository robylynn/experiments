#ifndef _FRAMEWORK_GEOMETRY_TRIANGLE_MESH_GEOMETRY_PROVIDER_ADAPTOR_H_
#define _FRAMEWORK_GEOMETRY_TRIANGLE_MESH_GEOMETRY_PROVIDER_ADAPTOR_H_

#include <type_traits>

#include <boost/iterator/counting_iterator.hpp>
#include <boost/iterator/filter_iterator.hpp>
#include <boost/iterator/transform_iterator.hpp>

#include <CGAL/Surface_mesh_complex_2_in_triangulation_3.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/Triangulation_2.h>

#include "vertexElement.h"

// Typetraits for mesh kernels. By default, the type of the Kernel for the mesh
// is simply forwarded. This assumes that the user passes in a correct
// KernelType that is derivable at calling location.
template <typename Triangulation, typename VertexElement>
struct MeshProviderTraits {};

template <typename Triangulation>
struct MeshProviderTraits<Triangulation, PositionVertexElement> {
  using type = Kernel::Point_3;
};

template <typename Triangulation>
struct MeshProviderTraits<Triangulation, ColorVertexElement> {
  using type = CGAL::Color;
};

template <typename Kernel>
struct MeshProviderTraits<CGAL::Polyhedron_3<Kernel>, PositionVertexElement> {
  using type = typename Kernel::Point_3;
};

// Code to adapt some common triangle mesh containers of CGAL to use a uniform
// interface for consumption by the geometry provider. Specialize this to adapt
// mesh geometry provider to provide renderable geometry for custom types.
// The base template provider for all "Triangulation" classes.
template <typename Triangulation, typename VE = PositionVertexElement,
          typename VT = typename MeshProviderTraits<Triangulation, VE>::type>
class TriangleMeshGeometryProviderAdaptor {
  using MeshType = Triangulation;

 public:
  using facet_iterator = typename MeshType::Finite_faces_iterator;
  using value_type = VT;
  using facet_vertex_iterator =
      boost::transform_iterator<std::function<const value_type&(int)>,
                                boost::counting_iterator<int>>;

  // Hold a facet, and, upon dereference, use the stored facet and the passed
  // in counting iterator index to yield the vertex being referred to in
  // the triangulation data-structure.
  struct FacetHolderFunctor {
    FacetHolderFunctor(facet_iterator fIter) : facetIter(fIter) {}

    template <typename VertexElement = VE>
    const value_type& operator()(int index) const {
      return operator()(index, VertexElement());
    }

    const value_type& operator()(int index,
                                 const PositionVertexElement& /**/) const {
      return facetIter->vertex(index)->point();
    }

    const value_type& operator()(int index,
                                 const ColorVertexElement& /**/) const {
      return facetIter->vertex(index)->info();
    }

    facet_iterator facetIter;
  };

  // For each triangle face, loop over the vertices
  facet_vertex_iterator fv_iterator(facet_iterator facetIter) const {
    FacetHolderFunctor facetHolder(facetIter);
    return boost::make_transform_iterator(boost::counting_iterator<int>(0),
                                          facetHolder);
  }

  const value_type& dereference(facet_vertex_iterator vertexIter) const {
    return *vertexIter;
  }

  facet_vertex_iterator next(facet_vertex_iterator vertexIter) const {
    return ++vertexIter;
  }

  size_t size(const MeshType* mesh) const { return mesh->number_of_faces(); }

  facet_iterator facets_begin(const MeshType* mesh) const {
    return mesh->finite_faces_begin();
  }

  facet_iterator facets_end(const MeshType* mesh) const {
    return mesh->finite_faces_end();
  }
};

// If the MeshRepresentation is based on HalfEdge data-structure, we can reuse
// some of the code.
template <typename T, typename VE, typename VT, typename FI, typename FVI>
class HalfEdgeBasedGeometryProviderAdaptor {
 public:
  using facet_iterator = FI;
  using facet_vertex_iterator = FVI;
  using value_type = VT;

  facet_vertex_iterator fv_iterator(facet_iterator facetIter) const {
    return facetIter->halfedge();
  }

  const value_type& dereference(facet_vertex_iterator vertexIter) const {
    return dereferenceInternal(vertexIter);
  }

  template <typename VertexElement = VE>
  const value_type& dereferenceInternal(
      const facet_vertex_iterator& vertexIter) const {
    return dereferenceInternal(vertexIter, VertexElement());
  }

  const value_type& dereferenceInternal(
      const facet_vertex_iterator& vertexIter,
      const PositionVertexElement& /**/) const {
    return vertexIter->vertex()->point();
  }

  const value_type& dereferenceInternal(const facet_vertex_iterator& vertexIter,
                                        const ColorVertexElement& /**/) const {
    return vertexIter->vertex()->info();
  }

  facet_vertex_iterator next(facet_vertex_iterator vertexIter) const {
    return vertexIter->next();
  }
};

// Specialize for Polyhedrons
template <typename Kernel, typename VE>
class TriangleMeshGeometryProviderAdaptor<CGAL::Polyhedron_3<Kernel>, VE>
    : public HalfEdgeBasedGeometryProviderAdaptor<
          TriangleMeshGeometryProviderAdaptor<CGAL::Polyhedron_3<Kernel>, VE>,
          VE, typename MeshProviderTraits<CGAL::Polyhedron_3<Kernel>, VE>::type,
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

// Specialization that handles CGAL generated surface meshes that rely on a 3D
// triangulation algorithm. The facet for such results is a <tetrahedron cell,
// vertex index> pair. We need to iterate over all the vertices not equal to
// the vertex index.
template <typename Triangulation, typename VE>
class TriangleMeshGeometryProviderAdaptor<
    CGAL::Surface_mesh_complex_2_in_triangulation_3<Triangulation>, VE> {
  using MeshType =
      CGAL::Surface_mesh_complex_2_in_triangulation_3<Triangulation>;

 public:
  using facet_iterator = typename MeshType::Facet_iterator;
  using value_type = typename MeshProviderTraits<Triangulation, VE>::type;
  using facet_vertex_iterator = boost::transform_iterator<
      std::function<const value_type&(int)>,
      boost::filter_iterator<std::function<int(int)>,
                             boost::counting_iterator<int>>>;

  // Hold a facet, and, upon dereference, use the stored facet and the passed
  // in counting iterator index to look yield the vertex being referred to in
  // the triangulation data-structure.
  struct FacetHolderFunctor {
    FacetHolderFunctor(facet_iterator fIter) : facetIter(fIter) {}

    template <typename VertexElement = VE>
    const value_type& operator()(int index) const {
      return operator()(index, VertexElement());
    }

    const value_type& operator()(int index,
                                 const PositionVertexElement& /**/) const {
      return facetIter->first->vertex(index)->point();
    }

    const value_type& operator()(int index,
                                 const ColorVertexElement& /**/) const {
      return facetIter->first->vertex(index)->info();
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

  const value_type& dereference(facet_vertex_iterator vertexIter) const {
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

#endif  // _FRAMEWORK_GEOMETRY_TRIANGLE_MESH_GEOMETRY_PROVIDER_ADAPTOR_H_
