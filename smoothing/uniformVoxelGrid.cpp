#include <CGAL/point_generators_3.h>

#include "uniformVoxelGrid.h"

UniformVoxelGrid::UniformVoxelGrid(GeometryType extent, size_t indexExtent) {
  CGAL::Creator_uniform_3<GeometryType, Kernel::Point_3> creator;
  CGAL::points_on_cube_grid_3(extent, indexExtent * indexExtent * indexExtent,
                              std::back_inserter(m_points), creator);
}
