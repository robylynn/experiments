#ifndef _UNIFORM_GRID_H_
#define _UNIFORM_GRID_H_

#include <CGAL/Point_3.h>
#include <CGAL/Iso_cuboid_3.h>
#include <CGAL/Origin.h>

#include "commonTypes.h"
#include "geometryTypes.h"

// Represents a uniform voxel grid of cubical voxels.
// Supports the following queries:
// a) Query bounds of the voxel situated at query point - const time
// b) Iteration over voxels (representation by points placed at the centers)
class UniformVoxelGrid {
 private:
  using ContainerType = std::vector<Kernel::Point_3>;
  // The points are located at the centers of the grid.
  ContainerType m_points;
  // The value of one dimension (all are the same) of the grid.
  GeometryType m_gridIncrement;
  // The bounds on the indexes of the voxel grid.
  Integer_3 m_indexBounds;

 public:
  // Rendering hint for rendering logic -- the max number of points this voxel
  // grid may possesses
  static constexpr int HINT_MAX_BOUND = 10000;

  // Cubical grid centered at origin spanning a region of space given by the
  // geometric and index extents (of a single dimension).
  UniformVoxelGrid(GeometryType extent, size_t indexExtent);

  // Obtains the bounds of the voxel that the location lies in. To uniquely
  // identify a voxel even for points that that lie on a face / edge /vertex of
  // voxels, a voxel is taken to be closed below, and open above. Querying
  // points outside the voxel grid returns a 0 sized bounding box
  Kernel::Iso_cuboid_3 voxelBoundsForLocation(
      const Kernel::Point_3& location) const;

  // The size of a voxel grid is the total number of voxels it contains
  size_t size() const { return m_points.size(); }

  // The shape of a voxel grid is a tuple of the number of voxels in each
  // dimension
  Integer_3 shape() const { return m_indexBounds; }
  size_t shape(size_t dimension) const {
    if (dimension == 0) return std::get<0>(m_indexBounds);
    if (dimension == 1) return std::get<1>(m_indexBounds);
    if (dimension == 2) return std::get<2>(m_indexBounds);
  }

  // The uniform voxel grid allows for iteration over itself by pass through to
  // container
  auto begin() -> decltype(m_points.begin()) { return m_points.begin(); }
  auto begin() const -> decltype(m_points.begin()) { return m_points.begin(); }
  auto end() -> decltype(m_points.end()) { return m_points.end(); }
  auto end() const -> decltype(m_points.end()) { return m_points.end(); }

  using value_type = Kernel::Point_3;
  using const_iterator = ContainerType::const_iterator;

 private:
  // Return if a particular index is valid
  bool isValidIndex(const Index_3& index) {
    return PartialOrder<Index_3>()(index, m_indexBounds);
  }
};

#endif  //_UNIFORM_GRID_H_
