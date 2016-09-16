#ifndef _POLYLOOP_H_
#define _POLYLOOP_H_

#include <vector>
#include <CGAL/circulator.h>

#include "CGALKernelSelector.h"

// A polyloop is an ordered sequence of polylines. Representation wise, a
// polyloop is representable using a container of points, with adjacent points
// implicitly understood to be connected to each other.
template <class PointType>
class Polyloop {
private:
  typedef std::vector<PointType> PointsContainer;
  PointsContainer m_points;
  typename PointsContainer::iterator m_insertPos;

public:
  Polyloop() : m_insertPos(m_points.end()) {}

  // Add a point at specified location to the Polyloop.
  void addPoint(typename PointsContainer::iterator iter,
                const PointType& point) {
    m_insertPos = m_points.insert(iter, point);
  }

  // Add a point after the last added point. If no point in the Polyloop, add
  // point as first point.
  void addPoint(const PointType& point) {
    m_insertPos = m_points.insert(m_insertPos, point);
  }

  typename PointsContainer::iterator removePoint(
                                    typename PointsContainer::iterator iter) {
    return m_points.erase(iter);
  }

  void updatePoint(typename PointsContainer::iterator iter,
                   const PointType& point) {
    m_points.assign(iter, iter+1, point);
  }

  // The polyloop allows for iteration over points by pass through to container.
  auto begin() -> decltype(m_points.begin()) {
    return m_points.begin();
  }

  auto begin() const -> decltype(m_points.begin()) {
    return m_points.begin();
  }

  auto end() -> decltype(m_points.end()) {
    return m_points.end();
  }

  auto end() const -> decltype(m_points.end()) {
      return m_points.end();
  }
};

#endif //_POLYLOOP_H_
