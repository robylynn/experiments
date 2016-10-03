#ifndef _UNIFORM_VOXEL_GRID_GEOMETRY_PROVIDER_H_
#define _UNIFORM_VOXEL_GRID_GEOMETRY_PROVIDER_H_

#include "geometryTypes.h"

#include <OGRE/Ogre.h>
#include <OGRE/OgreSimpleRenderable.h>

#include "geometryConstants.h"

#include "alignedCuboid.h"
#include "uniformVoxelGrid.h"
#include "sequentialGeometryRenderable.h"

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
      : public boost::iterator_facade<VertexIterator, Kernel::Point_3,
                                      boost::forward_traversal_tag> {
   public:
    VertexIterator(VoxelGridCubeProvider* cubeProvider, bool fEnd = false)
        : m_cubeProvider(cubeProvider),
          m_vertexIndex(0),
          m_voxelIterator(m_cubeProvider->m_voxelGrid.begin()),
          m_triangleBuilder(m_cubeProvider->m_voxelGrid.voxelBoundsForLocation(
              *m_voxelIterator)),
          m_triangleIterator(m_triangleBuilder.begin()) {
      if (fEnd) {
        m_voxelIterator = m_cubeProvider->m_voxelGrid.end();
        m_triangleIterator = m_triangleBuilder.end();
        m_vertexIndex = VERTICES_PER_TRIANGLE;
      }
    }

   private:
    friend class boost::iterator_core_access;
    void increment() {
      if (m_triangleIterator + 1 ==
              AlignedCuboidTrianglesBuilder::TRIANGLES_PER_CUBOID &&
          m_vertexIndex + 1 == VERTICES_PER_TRIANGLE) {
        m_triangleIndex = -1;
        // Create a new triangle builder attached to a different cuboid
        m_triangleBuilder = AlignedCuboidTrianglesBuilder(
            m_cubeProvider->m_voxelGrid.voxelBoundsForLocation(
                *m_voxelIterator));
      }
      if (m_vertexIndex + 1 == VERTICES_PER_TRIANGLE) {
        ++m_triangleIndex;
        ++m_triangleIterator;
        m_vertexIndex = -1;
      }
      ++m_vertexIndex;
    }

    bool equal(const VertexIterator& other) {
      return (m_voxelIterator == other.m_voxelIterator) &&
             (m_triangleIterator == other.m_triangleIterator) &&
             (m_vertexIndex == other.m_vertexIndex) &&
             (m_cubeProvider == other.m_cubeProvider);
    }

    const Kernel::Point_3& dereference() {
      return *m_triangleIterator[m_vertexIndex];
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
           AlignedCuboidTriangleBuilder::TRIANGLES_PER_CUBOID *
           VERTICES_PER_TRIANGLE;
  }

  VertexIterator begin() { return VertexIterator(); }
  VertexIterator end() { return VertexIterator(true /*fEnd*/); }
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
