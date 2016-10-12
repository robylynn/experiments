#ifndef _LEVELSETMESHBUILDER_H_
#define _LEVELSETMESHBUILDER_H_

#include <type_traits>

#include <CGAL/Robust_circumcenter_traits_3.h>
#include <CGAL/Triangulation_cell_base_with_circumcenter_3.h>
#include <CGAL/Delaunay_triangulation_3.h>
#include <CGAL/Surface_mesh_complex_2_in_triangulation_3.h>
#include <CGAL/make_surface_mesh.h>
#include <CGAL/Implicit_surface_3.h>

#include "geometryTypes.h"

// A mesh builder policy for level sets provides typedefs and instantiates a
// MeshingCriteria object that is inherited by the MeshBuilder.
class DefaultLevelSetMeshBuilderPolicy {
 private:
  // Some typedefs for the triangulation data structure
  // TODO msati3: Use CGAL::Robust_circumcenter_traits_3<Kernel> for robustness?
  using K = Kernel;
  // Vertex and cell types
  using Vb = CGAL::Surface_mesh_vertex_base_3<K>;
  using Cb = CGAL::Surface_mesh_cell_base_3<K>;
  using Cb_with_circumcenter =
      CGAL::Triangulation_cell_base_with_circumcenter_3<K, Cb>;
  // Triangulation Data structure
  using Tds = CGAL::Triangulation_data_structure_3<Vb, Cb_with_circumcenter>;

 public:
  // The triangulation algorithm used for surface mesh generation. Delaunay
  // triangulation with the current kernel is a good default choice.
  using MeshTriangulation = CGAL::Delaunay_triangulation_3<Kernel, Tds>;
  // Some parameters for meshing
  using MeshingCriteria =
      CGAL::Surface_mesh_default_criteria_3<MeshTriangulation>;

  // We will build a surface mesh. Choose an appropriate mesh
  // representation (half edge).
  using MeshRepresentation =
      CGAL::Surface_mesh_complex_2_in_triangulation_3<MeshTriangulation>;

  // The type of the sampling function.
  using SamplingFunction = std::function<Kernel::FT(const Kernel::Point_3&)>;

  using Surface_3 = CGAL::Implicit_surface_3<MeshTriangulation::Geom_traits,
                                             SamplingFunction>;

  // Populate the meshing criteria parameters with the lower angular bound for
  // each facet of the mesh, upper radius bound for surface Delaunay balls and
  // the upper distance bound between circumcenter of each face, and its
  // surface Delaunay ball.
  DefaultLevelSetMeshBuilderPolicy() : meshingCriteria(30, 0.1, 0.1) {}

 protected:
  MeshingCriteria meshingCriteria;
};

// Given an object that models a 3D scalar field (can be queried at a point for
// a value), a bounding sphere within which the scalar field is to be sampled,
// and a level set value, generates a mesh that corresponds to the level set
// value.
template <typename BuildPolicy = DefaultLevelSetMeshBuilderPolicy>
class LevelSetMeshBuilder : public BuildPolicy {
 private:
  using Triangulation = typename BuildPolicy::MeshTriangulation;
  using Representation = typename BuildPolicy::MeshRepresentation;

 public:
  LevelSetMeshBuilder() {}

  void buildMesh(
      const typename BuildPolicy::SamplingFunction& fieldSamplingFunction,
      const Kernel::Sphere_3& boundingSphere, double value) {
    Triangulation triangulation;
    Representation rep(triangulation);

    // TODO msati3: Extensible form of this? To allow not just sampling
    // implicit functions?
    typename BuildPolicy::Surface_3 surface(fieldSamplingFunction,
                                            boundingSphere);

    CGAL::make_surface_mesh(rep, surface, this->meshingCriteria,
                            CGAL::Manifold_with_boundary_tag());

    std::cout << "Number of vertices in the created mesh "
              << triangulation.number_of_vertices() << std::endl;
  }

  // Enforce that the geometry traits types for the triangulation are the same
  // as the currently selected Kernel's types
  /*static_assert(
      !std::is_same<typename Triangulation::Geom_traits::FT, Kernel::FT>::value,
      "The field types of the MeshTriangulation's geometry traits "
      "and the CGAL kernel selected by the application should be "
      "the same");
  static_assert(
      !std::is_same<typename Triangulation::Geom_traits::Point_3,
                    Kernel::Point_3>::value,
      "The point types of the MeshTriangulation's geometry traits and the "
      "CGAL kernel selected by the application should be the same");*/
};

#endif  //_LEVELSETMESHBUILDER_H_
