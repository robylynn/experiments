#ifndef _DISTANCE_FIELD_COMPUTERS_H_
#define _DISTANCE_FIELD_COMPUTERS_H_

#include <boost/variant/static_visitor.hpp>

#include <geometryTypes.h>

// Given a sample location in R3, computes the scalar field that is equal to the
// squared distance of the sample from a given geometry representation.
class SquaredDistanceFieldComputer : public boost::static_visitor<Kernel::FT> {
 public:
  using result_type = Kernel::FT;

  SquaredDistanceFieldComputer(const Kernel::Point_3& point)
      : m_point(&point) {}
  // The distance field computer doesn't own any of the passed in points.
  SquaredDistanceFieldComputer(const Kernel::Point_3&& point) = delete;

  template <typename RepType>
  Kernel::FT operator()(const RepType& rep) const {
    return CGAL::squared_distance(rep, *m_point);
  }

 private:
  const Kernel::Point_3* m_point;
};

/*
// Given a sample location in R3, computes the scalar field that is equal to the
// distance of the sample from a given geometry representation.
class DistanceFieldComputer : public boost::static_visitor<Kernel::FT> {
 public:
  using ComputedFieldType = Kernel::FT;

  DistanceFieldComputer();

  template <typename RepType>
  Kernel::FT operator()(const RepType& geometryRep,
                        const Kernel::Point_3& point) {
    return sqrt(CGAL::squared_distance(geometryRep, point));
  }

  template <typename WrappedType>
  Kernel::FT operator()<std::reference_wrapper<WrappedType>>(
      const std::reference_wrapper<WrappedType>& rep,
      const Kernel::Point_3& point) {
    return sqrt(CGAL::squared_distance(rep.get(), m_point));
  }
};
*/

#endif  //_DISTANCE_FIELD_COMPUTERS_H_
