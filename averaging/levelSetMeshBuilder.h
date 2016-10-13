#ifndef _LEVELSETMESHBUILDER_H_
#define _LEVELSETMESHBUILDER_H_

#include <type_traits>

#include <glog/logging.h>

#include <CGAL/Robust_circumcenter_traits_3.h>
#include <CGAL/Triangulation_cell_base_with_circumcenter_3.h>
#include <CGAL/Delaunay_triangulation_3.h>
#include <CGAL/Surface_mesh_complex_2_in_triangulation_3.h>
#include <CGAL/make_surface_mesh.h>
#include <CGAL/Implicit_surface_3.h>

#include "geometryTypes.h"

// A scalar field sampling function must model this concept
template <typename KernelType>
struct ScalarFieldPointSampler {
  using type = std::function<typename KernelType::FT(
      const typename KernelType::Point_3&)>;
};

// A mesh builder policy for level sets provides typedefs and instantiates a
// MeshingCriteria object that is inherited by the MeshBuilder.
class DefaultLevelSetMeshBuilderPolicy {
 private:
  // Some typedefs for the triangulation data structure
  using RobustKernel = CGAL::Robust_circumcenter_traits_3<Kernel>;
  // A 3D triangulation requires types modeling the vertex base concepts
  // and cell base (a cell is a maximal simplex -- a tetrahedron in 3D)
  using Vb = CGAL::Surface_mesh_vertex_base_3<RobustKernel>;
  using Cb = CGAL::Surface_mesh_cell_base_3<RobustKernel>;
  using Cb_with_circumcenter =
      CGAL::Triangulation_cell_base_with_circumcenter_3<RobustKernel, Cb>;
  // 3D Triangulation Data structure
  using Tds = CGAL::Triangulation_data_structure_3<Vb, Cb_with_circumcenter>;

 public:
  // The triangulation algorithm used for surface mesh generation. Delaunay
  // triangulation with the current kernel is a good default choice.
  using MeshTriangulation = CGAL::Delaunay_triangulation_3<RobustKernel, Tds>;
  // Some parameters for meshing
  using MeshingCriteria =
      CGAL::Surface_mesh_default_criteria_3<MeshTriangulation>;

  // We will build a surface mesh. Choose an appropriate mesh representation for
  // the restricted Delaunay triangulation used by the surface mesh generator.
  using MeshRepresentation =
      CGAL::Surface_mesh_complex_2_in_triangulation_3<MeshTriangulation>;

  // The kernel that will decide the sampling function that is desired by the
  // MeshBuilder policy
  using DesiredSamplingKernel = RobustKernel;

  // The surface builder is driven by the above configured types
  using Surface_3 = CGAL::Implicit_surface_3<
      MeshTriangulation::Geom_traits,
      typename ScalarFieldPointSampler<DesiredSamplingKernel>::type>;

  // Populate the meshing criteria parameters with the lower angular bound for
  // each facet of the mesh, upper radius bound for surface Delaunay balls and
  // the upper distance bound between circumcenter of each face, and its
  // surface Delaunay ball.
  DefaultLevelSetMeshBuilderPolicy() : meshingCriteria(30, 1, 1) {}

 protected:
  MeshingCriteria meshingCriteria;
};

// Adapts provided sampling function to the type expected by the
// LevelSetMeshBuilderPolicy
template <typename DesiredKernel>
class ScalarFieldSamplingAdaptor {
 private:
  typename ScalarFieldPointSampler<DesiredKernel>::type m_adaptedFunction;

 public:
  ScalarFieldSamplingAdaptor(
      const ScalarFieldPointSampler<Kernel>::type& samplingFunction) {
    m_adaptedFunction =
        [this, samplingFunction](const typename DesiredKernel::Point_3& point) {
          return adaptFieldValue(samplingFunction(adaptPoint(point)));
        };
  }

  auto get() -> decltype(m_adaptedFunction) { return m_adaptedFunction; }

 private:
  Kernel::Point_3 adaptPoint(const typename DesiredKernel::Point_3& point) {
    return Kernel::Point_3(point.x(), point.y(), point.z());
  }

  typename DesiredKernel::FT adaptFieldValue(Kernel::FT value) {
    return static_cast<typename DesiredKernel::FT>(value);
  }
};

// The adaptor is not necessary in case we are on the same Kernel. In this
// case, simply return that same function.
template <>
class ScalarFieldSamplingAdaptor<Kernel> {
 private:
  typename ScalarFieldPointSampler<Kernel>::type m_adaptedFunction;

 public:
  ScalarFieldSamplingAdaptor(
      const ScalarFieldPointSampler<Kernel>::type& samplingFunction) {
    m_adaptedFunction = samplingFunction;
  }

  auto get() -> decltype(m_adaptedFunction) { return m_adaptedFunction; }
};

// Given an object that models a 3D scalar field (can be queried at a point for
// a value), a bounding sphere within which the scalar field is to be sampled,
// and a level set value, generates a mesh that corresponds to the level set
// value.
template <typename BuildPolicy = DefaultLevelSetMeshBuilderPolicy>
class LevelSetMeshBuilder : public BuildPolicy {
 private:
  using Triangulation = typename BuildPolicy::MeshTriangulation;
  using SamplingFunction = typename ScalarFieldPointSampler<Kernel>::type;

 public:
  using Representation = typename BuildPolicy::MeshRepresentation;
  Representation buildMesh(const SamplingFunction& fieldSamplingFunction,
                           const Kernel::Sphere_3& boundingSphere,
                           double value) {
    Triangulation triangulation;
    Representation rep(triangulation);

    // TODO msati3: Extensible form of this to allow not just sampling
    // implicit functions possible?
    ScalarFieldSamplingAdaptor<typename BuildPolicy::DesiredSamplingKernel>
        samplingAdaptor(fieldSamplingFunction);

    typename BuildPolicy::Surface_3 surface(samplingAdaptor.get(),
                                            boundingSphere);

    CGAL::make_surface_mesh(rep, surface, this->meshingCriteria,
                            CGAL::Manifold_with_boundary_tag());

    LOG(INFO)
        << "Created level set mesh. Number of vertices in the created mesh = "
        << triangulation.number_of_vertices() << std::endl;
    return rep;
  }
};

#endif  //_LEVELSETMESHBUILDER_H_
