#ifndef _FRAMEWORK_GEOMETRY_SAMPLERS_UNIFORM_PLANAR_GRID_SAMPLER_H_
#define _FRAMEWORK_GEOMETRY_SAMPLERS_UNIFORM_PLANAR_GRID_SAMPLER_H_

#include <boost/iterator/iterator_facade.hpp>

#include <CGAL/Origin.h>
#include <CGAL/Plane_3.h>
#include <CGAL/Regular_grid_2.h>

#include "geometryTypes.h"

// Generates samples on a uniform grid on the plane specified. The user can
// provide three points that are counterclockwise oriented in the plane to
// obtain a unique orthogonal basis that is used to generate the grid. Else, we
// generate an arbitrary basis on the user specified plane.
class UniformPlanarGridSampler {
 public:
  UniformPlanarGridSampler(const Kernel::Plane_3& plane, size_t xSamples,
                           size_t ySamples, Kernel::FT xSize, Kernel::FT ySize)
      : m_plane(plane), m_gridSize(xSamples, ySamples) {
    populateGridParams(plane.base1(), plane.base2(), xSamples, ySamples, xSize,
                       ySize);
  }

  UniformPlanarGridSampler(const Kernel::Point_3& point1,
                           const Kernel::Point_3& point2,
                           const Kernel::Point_3& point3, size_t xSamples,
                           size_t ySamples, Kernel::FT xSize, Kernel::FT ySize)
      : m_plane(point1, point2, point3), m_gridSize(xSamples, ySamples) {
    populateGridParams(Kernel::Vector_3(point1, point2),
                       Kernel::Vector_3(point1, point3), xSamples, ySamples,
                       xSize, ySize);
  }

  class PointGenerator
      : public boost::iterator_facade<PointGenerator, const Kernel::Point_3,
                                      std::input_iterator_tag,
                                      const Kernel::Point_3> {
   public:
    explicit PointGenerator(const UniformPlanarGridSampler& grid)
        : m_grid(grid),
          m_currentLocation(m_grid.m_startLocation + m_grid.m_uIncrement * 0.5 +
                            m_grid.m_vIncrement * 0.5),
          m_uIncrIndex(0),
          m_vIncrIndex(0) {}

    PointGenerator(const UniformPlanarGrid& grid, size_t uIncrIndex,
                   size_t vIncrIndex)
        : m_grid(grid), m_uIncrIndex(uIncrIndex), m_vIncrIndex(vIncrIndex) {}

   private:
    friend class boost::iterator_core_access;

    void increment() {
      if (++m_uIncrIndex < std::get<0>(m_grid.m_gridSize)) {
        m_currentLocation = m_currentLocation + m_grid.m_uIncrement;
      } else {
        if (++m_vIncrIndex < std::get<1>(m_grid.m_gridSize)) {
          m_uIncrIndex = 0;
          m_currentLocation = m_grid.m_startLocation +
                              m_grid.m_uIncrement * 0.5 +
                              m_grid.m_vIncrement * (m_vIncrIndex + 0.5);
        }
      }
    }

    // Note the currentLocation is just additional state for faster grid
    // iteration. It is not used for equality checking of the iterators.
    bool equal(const PointGenerator& other) const {
      return ((m_uIncrIndex == other.m_uIncrIndex) &&
              (m_vIncrIndex == other.m_vIncrIndex) &&
              (&m_grid == &other.m_grid));
    }

    const Kernel::Point_3 dereference() const { return m_currentLocation; }

    const UniformPlanarGrid& m_grid;
    mutable Kernel::Point_3 m_currentLocation;
    size_t m_uIncrIndex;
    size_t m_vIncrIndex;
  };

  using const_iterator = PointGenerator;

  const_iterator begin() const { return PointGenerator(*this); }

  const_iterator end() const {
    return PointGenerator(*this, std::get<0>(m_gridSize),
                          std::get<1>(m_gridSize));
  }

  size_t size() const {
    return std::get<0>(m_gridSize) * std::get<1>(m_gridSize);
  }

  // Return four points that correspond to the corners of the grid
  std::vector<Kernel::Point_3> gridCorners() {
    return {m_startLocation,
            m_startLocation + m_uIncrement * std::get<0>(m_gridSize),
            m_startLocation + m_uIncrement * std::get<0>(m_gridSize) +
                m_vIncrement * std::get<1>(m_gridSize),
            m_startLocation + m_vIncrement * std::get<1>(m_gridSize)};
  }

 private:
  void populateGridParams(const Kernel::Vector_3& u, const Kernel::Vector_3& v,
                          size_t xSamples, size_t ySamples, Kernel::FT xSize,
                          Kernel::FT ySize) {
    CGAL_precondition(xSize != 0);
    CGAL_precondition(ySize != 0);
    CGAL_precondition(xSamples != 0);
    CGAL_precondition(ySamples != 0);
    CGAL_precondition(u.squared_length() != 0);
    CGAL_precondition(v.squared_length() != 0);

    m_uIncrement = (xSize / xSamples) * u;
    m_vIncrement = (ySize / ySamples) * v;

    m_startLocation = CGAL::Origin() - ((xSamples / 2.0) * m_uIncrement +
                                        (ySamples / 2.0) * m_vIncrement);
  }

  Kernel::Plane_3 m_plane;
  Kernel::Vector_3 m_uIncrement;
  Kernel::Vector_3 m_vIncrement;
  Kernel::Point_3 m_startLocation;
  std::tuple<size_t, size_t> m_gridSize;
};

using UniformPlanarGridGenerator = UniformPlanarGridSampler;

#endif  //_FRAMEWORK_GEOMETRY_SAMPLERS_UNIFORM_PLANAR_GRID_SAMPLER_H_
