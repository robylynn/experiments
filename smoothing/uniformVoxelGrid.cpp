#include <CGAL/point_generators_3.h>

#include "uniformVoxelGrid.h"

UniformVoxelGrid::UniformVoxelGrid(GeometryType extent, size_t indexExtent)
    : m_indexBounds(indexExtent, indexExtent, indexExtent) {
  CGAL::Creator_uniform_3<GeometryType, Kernel::Point_3> creator;
  CGAL::points_on_cube_grid_3(extent, indexExtent * indexExtent * indexExtent,
                              std::back_inserter(m_points), creator);
}

CGAL::Bbox_3 UniformVoxelGrid::voxelBoundsForLocation(
    const Kernel::Point_3& location) const {
  // Scale by the grid increment, and round down.
  Kernel::Vector_3 vector = (location - CGAL::ORIGIN) / m_gridIncrement;

  Kernel::Vector_3 minPoint((int)vector.x() * m_gridIncrement,
                            (int)vector.y() * m_gridIncrement,
                            (int)vector.z() * m_gridIncrement);
  Kernel::Vector_3 maxPoint(((int)vector.x() + 1) * m_gridIncrement,
                            ((int)vector.y() + 1) * m_gridIncrement,
                            ((int)vector.z() + 1) * m_gridIncrement);
  return CGAL::Bbox_3(minPoint.x(), minPoint.y(), minPoint.z(), maxPoint.x(),
                      maxPoint.y(), maxPoint.z());
}
