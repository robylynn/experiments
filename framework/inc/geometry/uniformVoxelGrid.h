#ifndef _FRAMEWORK_GEOMETRY_UNIFORM_GRID_H_
#define _FRAMEWORK_GEOMETRY_UNIFORM_GRID_H_

#include <glog/logging.h>

#include <CGAL/Point_3.h>
#include <CGAL/Iso_cuboid_3.h>
#include <CGAL/Origin.h>

#include <commonTypes.h>
#include <partialOrder.h>

#include "geometryTypes.h"

/** Represents a uniform voxel grid of cubical voxels, with voxels being
 * represented by points
 * Supports the following queries:
 * a) Query bounds of the voxel situated at query point - const time
 * b) Iteration over voxels (representation by points placed at the centers)
 */
template <typename VertexBase = std::tuple<PositionAttribute_3>>
class UniformVoxelGrid
    : public VertexAttribsIters<UniformVoxelGrid<VertexBase>> {
 private:
  using ContainerType =
      VertexAttributeTraits < UniformVoxelGrid<VertexBase>::container_type;
  // The points are located at the centers of the grid.
  ContainerType m_vertexAttribs;
  // The value of one dimension (all are the same) of the grid.
  FieldType m_gridIncrement;
  // The bounds on the indexes of the voxel grid.
  Integer_3 m_indexBounds;

 public:
  using vertex_value_type = typename VertexAttributesContainer::value_type;
  using vertex_iterator = typename VertexAttributesContainer::iterator;
  using const_vertex_iterator =
      typename VertexAttributesContainer::const_iterator;

  // Cubical grid centered at origin spanning a region of space given by the
  // geometric range [-extent, extent] in each dimension, and the index extents
  // (of a single dimension).
  UniformVoxelGrid(FieldType extent, size_t indexExtent);

  // Obtains the bounds of the voxel that the location lies in. To uniquely
  // identify a voxel even for points that that lie on a face / edge /vertex of
  // voxels, a voxel is taken to be closed below, and open above. Querying
  // points outside the voxel grid returns a 0 sized bounding box
  Kernel::Iso_cuboid_3 voxelBoundsForLocation(
      const Kernel::Point_3& location) const;

  // The size of a voxel grid is the total number of voxels it contains
  size_t size() const { return m_vertexAttribs.size(); }

  // The shape of a voxel grid is a tuple of the number of voxels in each
  // dimension
  Integer_3 shape() const { return m_indexBounds; }
  size_t shape(size_t dimension) const {
    if (dimension == 0) return std::get<0>(m_indexBounds);
    if (dimension == 1) return std::get<1>(m_indexBounds);
    if (dimension == 2) return std::get<2>(m_indexBounds);
    LOG(ERROR) << "Coordinate dimension for uniform voxel grid is outside 3";
  }

  // The uniform voxel grid allows for iteration over itself by pass through to
  // container
  auto vertices_begin() -> decltype(m_vertexAttribs.begin()) {
    return m_vertexAttribs.begin();
  }
  auto vertices_begin() const -> decltype(m_vertexAttribs.begin()) {
    return m_vertexAttribs.begin();
  }
  auto vertices_end() -> decltype(m_vertexAttribs.end()) {
    return m_vertexAttribs.end();
  }
  auto vertices_end() const -> decltype(m_vertexAttribs.end()) {
    return m_vertexAttribs.end();
  }

 private:
  // Return if a particular index is valid
  bool isValidIndex(const Index_3& index) {
    return PartialOrder<Index_3>()(index, m_indexBounds);
  }
};

template <typename VertexBase>
struct VertexAttributeTraits<UniformVoxelGrid<VertexBase>> {
 public:
  using value_type = attributes_to_types_t<VertexBase>;
  using container_type = std::vector<value_type>;
  using iterator = typename container_type::iterator;
  using const_iterator = typename container_type::const_iterator;
};

#endif  //_FRAMEWORK_GEOMETRY_UNIFORM_GRID_H_
