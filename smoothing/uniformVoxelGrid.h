#ifndef _UNIFORM_GRID_H_
#define _UNIFORM_GRID_H_

#include <CGAL/Point_3.h>

#include "geometryTypes.h"

// Represents a uniform voxel grid
class UniformVoxelGrid {
 public:
  // Cubical grid spanning a region of space given by the geometric and index
  // extents (of a single dimension).
  UniformVoxelGrid(GeometryType extent, size_t indexExtent);

 private:
  // The points are located at the centers of the grid.
  std::vector<Kernel::Point_3> m_points;
};

#endif  //_UNIFORM_GRID_H_
