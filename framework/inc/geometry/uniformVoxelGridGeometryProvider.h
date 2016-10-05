#ifndef _UNIFORM_VOXEL_GRID_GEOMETRY_PROVIDER_H_
#define _UNIFORM_VOXEL_GRID_GEOMETRY_PROVIDER_H_

#include <OGRE/Ogre.h>
#include <OGRE/OgreSimpleRenderable.h>

#include "geometryConstants.h"
#include "geometryTypes.h"

#include "cuboidGeometryProvider.h"
#include "uniformVoxelGrid.h"

class VoxelGridPointProvider {
 private:
  const UniformVoxelGrid& m_voxelGrid;

 public:
  static constexpr Ogre::RenderOperation::OperationType PRIMITIVE_TYPE =
      Ogre::RenderOperation::OT_POINT_LIST;

  VoxelGridPointProvider(const UniformVoxelGrid& voxelGrid)
      : m_voxelGrid(voxelGrid) {}
  size_t size() const { return m_voxelGrid.size(); }

  auto begin() const -> decltype(m_voxelGrid.begin()) {
    return m_voxelGrid.begin();
  }
  auto end() const -> decltype(m_voxelGrid.end()) { return m_voxelGrid.end(); }
};

class VoxelGridCubeProvider {
 private:
  const UniformVoxelGrid& m_voxelGrid;

 public:
  // TODO msati3 - the max bound is a dynamic property that will depend on each
  // object.
  static constexpr Ogre::RenderOperation::OperationType PRIMITIVE_TYPE =
      Ogre::RenderOperation::OT_TRIANGLE_LIST;

  VoxelGridCubeProvider(const UniformVoxelGrid& voxelGrid)
      : m_voxelGrid(voxelGrid) {}

  class VertexIterator
      : public boost::iterator_facade<VertexIterator, const Kernel::Point_3,
                                      boost::forward_traversal_tag> {
   public:
    // TODO msati3 perf: The voxelBoundsForLocation call may be slow. Need to
    // cache this then, and remove the m_triangleIndex logic, replacing it by
    // triangleBuilder's triangle iterator.
    VertexIterator(const VoxelGridCubeProvider* cubeProvider, bool fEnd = false)
        : m_cubeProvider(cubeProvider),
          m_vertexIndex(0),
          m_triangleIndex(0),
          m_voxelIterator(m_cubeProvider->m_voxelGrid.begin()),
          m_triangleBuilder(m_cubeProvider->m_voxelGrid.voxelBoundsForLocation(
              *m_voxelIterator)),
          m_triangleIterator(m_triangleBuilder.begin()) {
      if (fEnd) {
        m_vertexIndex = VERTICES_PER_TRIANGLE;
        m_triangleIndex = AlignedCuboidTrianglesBuilder::TRIANGLES_PER_CUBOID;
        m_voxelIterator = m_cubeProvider->m_voxelGrid.end();
      }
    }

   private:
    friend class boost::iterator_core_access;
    void increment() {
      if (m_vertexIndex + 1 == VERTICES_PER_TRIANGLE) {
        m_vertexIndex = -1;
        // Red herring: Ideally, a triangle index need not be maintained, and
        // this information should be obtainable from the triangle iterator.
        // However, there is no memory address related to the Cuboids returned
        // by the cubeProvider via the voxelGrid. This allows for incremental
        // creation of cuboids related to a voxel, without having them in
        // memory. However, the price we pay is that the TrianglesBuilder
        // instances are not retrievable, as there is no storage of them as
        // well. Thus, TriangleBuilder's triangle iterators cannot be compared,
        // for two VertexIterators, as the TriangleBuilder instances associated
        // with them would be different.
        // Thus, here we use the implicit uniqueness of a <voxelIterator,
        // triangleIndex, vertexIndex> tuple to obtain conditions for equality
        // and end of VertexIterator.
        ++m_triangleIterator;
        if (++m_triangleIndex ==
            AlignedCuboidTrianglesBuilder::TRIANGLES_PER_CUBOID) {
          if (++m_voxelIterator == m_cubeProvider->m_voxelGrid.end()) {
            // In this case, we are done with traversing the voxel grid and the
            // vertexIndex should be set to the index for the end iterator.
            // Thus, we set it to one less than this, as it is incremented at
            // the end of this function.
            m_vertexIndex = VERTICES_PER_TRIANGLE - 1;
          } else {  // There are more voxels in the voxel grid
            // Create a new triangle builder attached to a different cuboid
            m_triangleBuilder = AlignedCuboidTrianglesBuilder(
                m_cubeProvider->m_voxelGrid.voxelBoundsForLocation(
                    *m_voxelIterator));
            m_triangleIterator = m_triangleBuilder.begin();
            m_triangleIndex = 0;
          }
        }
      }
      ++m_vertexIndex;
    }

    bool equal(const VertexIterator& other) const {
      return (m_voxelIterator == other.m_voxelIterator) &&
             (m_triangleIndex == other.m_triangleIndex) &&
             (m_vertexIndex == other.m_vertexIndex) &&
             (m_cubeProvider == other.m_cubeProvider);
    }

    // Dereference to return Point directly, not a reference to it, as the CGAL
    // api doesn't allow for retrieving references.
    // TODO msati3 PERF: If this is slow, we might want to cache the returnined
    // point, and return a const reference if that helps.
    const Kernel::Point_3& dereference() const {
      return (*m_triangleIterator)[m_vertexIndex];
    }

    // A geometric point is uniquely identified by a voxel index, the
    // triangle index of the voxel under consideration, and further the
    // vertex index of the <voxel, triangle> tuple under consideration
    size_t m_triangleIndex;
    size_t m_vertexIndex;

    const VoxelGridCubeProvider* m_cubeProvider;
    UniformVoxelGrid::const_iterator m_voxelIterator;
    AlignedCuboidTrianglesBuilder m_triangleBuilder;
    AlignedCuboidTrianglesBuilder::const_iterator m_triangleIterator;
  };

  size_t size() const {
    return m_voxelGrid.size() *
           AlignedCuboidTrianglesBuilder::TRIANGLES_PER_CUBOID *
           VERTICES_PER_TRIANGLE;
  }
  VertexIterator begin() const { return VertexIterator(this); }
  VertexIterator end() const { return VertexIterator(this, true /*fEnd*/); }
};

// A geometry provider for the voxel grid representation. The voxel
// representation object itself must remain valid during the use of this
// adaptor class.
template <class ProviderPolicy = VoxelGridPointProvider>
class UniformVoxelGridGeometryProvider : public ProviderPolicy {
 public:
  static constexpr int HINT_MAX_BOUND = UniformVoxelGrid::HINT_MAX_BOUND;

  UniformVoxelGridGeometryProvider(const UniformVoxelGrid& voxelGrid);
  ~UniformVoxelGridGeometryProvider() {}
};

template <typename ProviderPolicy>
UniformVoxelGridGeometryProvider<ProviderPolicy>::
    UniformVoxelGridGeometryProvider(const UniformVoxelGrid& voxelGrid)
    : ProviderPolicy(voxelGrid) {}

#endif  //_UNIFORM_VOXEL_GRID_GEOMETRY_PROVIDER_H_
