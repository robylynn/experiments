#ifndef _FRAMEWORK_GEOMETRY_TRIANGLE_MESH_SIMPLICIAL_PROVIDER_API_ADAPTOR_H_
#define _FRAMEWORK_GEOMETRY_TRIANGLE_MESH_SIMPLICIAL_PROVIDER_API_ADAPTOR_H_

#include <type_traits>

#include <boost/iterator/counting_iterator.hpp>
#include <boost/iterator/filter_iterator.hpp>
#include <boost/iterator/transform_iterator.hpp>

#include <CGAL/Surface_mesh_complex_2_in_triangulation_3.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/Triangulation_2.h>

#include "attributes.h"
#include "simplexTypes.h"

// TODO msati3: Investigate how to furnish tuples of attributes for different
// mesh representations.
template <typename Triangulation, typename SimplexType, typename Attribute>
class TriangleMeshAPIAdaptor {};

// Code to adapt some common triangle mesh containers of CGAL to use a uniform
// interface for consumption by the geometry provider. Specialize this to adapt
// mesh geometry provider to provide renderable geometry for custom types.
// The base template provider for all "Triangulation" classes.
template <typename Triangulation, typename Attribute>
class TriangleMeshAPIAdaptor<Triangulation, TriangleList, Attribute> {
  using MeshType = Triangulation;

 public:
  using facet_iterator = typename MeshType::Finite_faces_iterator;
  using value_type = Attribute;
  using facet_vertex_iterator =
      boost::transform_iterator<std::function<const value_type&(int)>,
                                boost::counting_iterator<int>>;

  // Hold a facet, and, upon dereference, use the stored facet and the passed
  // in counting iterator index to yield the vertex being referred to in
  // the triangulation data-structure.
  struct FacetHolderFunctor {
    FacetHolderFunctor(facet_iterator fIter) : facetIter(fIter) {}

    template <typename AT = Attribute>
    const value_type& operator()(int index) const {
      return operator()(index, Attribute());
    }

    const value_type& operator()(int index,
                                 const PositionAttribute_3& /**/) const {
      return facetIter->vertex(index)->point();
    }

    const value_type& operator()(int index, const ColorAttribute& /**/) const {
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
template <typename T, typename AT, typename VT, typename FI, typename FVI>
class HalfEdgeBasedAPIAdaptor {
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

  template <typename Attribute = AT>
  const value_type& dereferenceInternal(
      const facet_vertex_iterator& vertexIter) const {
    return dereferenceInternal(vertexIter, Attribute());
  }

  const value_type& dereferenceInternal(const facet_vertex_iterator& vertexIter,
                                        const PositionAttribute_3& /**/) const {
    return vertexIter->vertex()->point();
  }

  const value_type& dereferenceInternal(const facet_vertex_iterator& vertexIter,
                                        const ColorAttribute& /**/) const {
    return vertexIter->vertex()->info();
  }

  facet_vertex_iterator next(facet_vertex_iterator vertexIter) const {
    return vertexIter->next();
  }
};

// Specialize for Polyhedrons
template <typename Kernel, typename Attribute>
class TriangleMeshAPIAdaptor<CGAL::Polyhedron_3<Kernel>, TriangleList,
                             Attribute>
    : public HalfEdgeBasedAPIAdaptor<
          TriangleMeshAPIAdaptor<CGAL::Polyhedron_3<Kernel>, TriangleList,
                                 Attribute>,
          Attribute, typename Attribute::type,
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
template <typename Triangulation, typename Attribute>
class TriangleMeshAPIAdaptor<
    CGAL::Surface_mesh_complex_2_in_triangulation_3<Triangulation>,
    TriangleList, Attribute> {
  using MeshType =
      CGAL::Surface_mesh_complex_2_in_triangulation_3<Triangulation>;

 public:
  using facet_iterator = typename MeshType::Facet_iterator;
  using value_type = typename Attribute::type;
  using facet_vertex_iterator = boost::transform_iterator<
      std::function<const value_type&(int)>,
      boost::filter_iterator<std::function<int(int)>,
                             boost::counting_iterator<int>>>;

  // Hold a facet, and, upon dereference, use the stored facet and the passed
  // in counting iterator index to look yield the vertex being referred to in
  // the triangulation data-structure.
  struct FacetHolderFunctor {
    FacetHolderFunctor(facet_iterator fIter) : facetIter(fIter) {}

    template <typename AT = Attribute>
    const value_type& operator()(int index) const {
      return operator()(index, AT());
    }

    const value_type& operator()(int index,
                                 const PositionAttribute_3& /**/) const {
      return facetIter->first->vertex(index)->point();
    }

    const value_type& operator()(int index, const ColorAttribute& /**/) const {
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

#endif  // _FRAMEWORK_GEOMETRY_TRIANGLE_MESH_SIMPLICIAL_PROVIDER_API_ADAPTOR_H_
