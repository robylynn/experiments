#ifndef _DISTANCE_FIELD_COMPUTERS_H_
#define _DISTANCE_FIELD_COMPUTERS_H_

#include <boost/variant/static_visitor.hpp>

#include <CGAL/Point_3.h>

#include <geometryTypes.h>
#include <squaredDistance.h>
#include <orientedSide.h>

// Given a sample location, computes the scalar field that is equal to the
// squared distance of the sample from a given geometry representation.
template <typename PointType>
class SquaredDistanceFieldComputer : public boost::static_visitor<Kernel::FT> {
 public:
  using result_type = Kernel::FT;

  SquaredDistanceFieldComputer(const PointType& point) : m_point(&point) {}
  // The distance field computer doesn't own any of the passed in points.
  SquaredDistanceFieldComputer(const PointType&& point) = delete;

  template <typename RepType>
  result_type operator()(const RepType& rep) const {
    return CGAL::squared_distance(rep, *m_point);
  }

 private:
  const PointType* m_point;
};

// Given a sample location in R3, computes the scalar field that is equal to the
// signed distance of the sample from a given geometry representation.
template <typename PointType>
class SignedDistanceFieldComputer : public boost::static_visitor<Kernel::FT> {
 public:
  using ComputedFieldType = Kernel::FT;

  SignedDistanceFieldComputer(const PointType& point) : m_point(&point) {}
  // This distance field computer doesn't own any of the passed in points
  SignedDistanceFieldComputer(const PointType&& point) = delete;

  template <typename RepType>
  Kernel::FT operator()(const RepType& geometryRep, const PointType& point) {
    CGAL::Oriented_side side = CGAL::oriented_side(geometryRep, point);
    Kernel::FT value = sqrt(CGAL::squared_distance(geometryRep, point));
    if (side == CGAL::ON_ORIENTED_BOUNDARY) return 0;
    return side == CGAL::ON_NEGATIVE_SIDE ? -value : value;
  }

 private:
  const PointType* m_point;
};

#endif  //_DISTANCE_FIELD_COMPUTERS_H_
