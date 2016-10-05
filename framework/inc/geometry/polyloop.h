#ifndef _POLYLOOP_H_
#define _POLYLOOP_H_

#include <vector>
#include <CGAL/circulator.h>

#include "geometryTypes.h"

// A polyloop is an ordered sequence of polylines. Representation wise, a
// polyloop is representable using a container of points, with adjacent points
// implicitly understood to be connected to each other.
template <typename PointType>
class Polyloop {
  using PointsContainer = std::vector<PointType>;

  PointsContainer m_points;

 public:
  using value_type = PointType;
  using iterator = typename PointsContainer::iterator;
  using const_iterator = typename PointsContainer::const_iterator;

  // A hint on the maximum size that a polyloop may have. This may be useful
  // for rendering in non-immediate mode.
  static constexpr size_t HINT_MAX_BOUND = 1000;

  Polyloop() {}

  // The size of a Polyloop is the number of points it contains
  size_t size() const { return m_points.size(); }

  // Add a point at specified location to the Polyloop.
  void addPoint(iterator iter, const PointType& point) {
    m_points.insert(iter, point);
  }

  // Add a point after the last point. If no point in the Polyloop, add
  // point as first point.
  void addPoint(const PointType& point) {
    m_points.push_back(point);
  }

  iterator removePoint(iterator iter) { return m_points.erase(iter); }

  void updatePoint(typename PointsContainer::iterator iter,
                   const PointType& point) {
    m_points.assign(iter, iter + 1, point);
  }

  // The polyloop allows for iteration over points by pass through to container,
  // and conforms to the CGAL MeshPolyline_3 concept
  auto begin() -> decltype(m_points.begin()) { return m_points.begin(); }
  auto begin() const -> decltype(m_points.begin()) { return m_points.begin(); }
  auto end() -> decltype(m_points.end()) { return m_points.end(); }
  auto end() const -> decltype(m_points.end()) { return m_points.end(); }
};

#endif  //_POLYLOOP_H_