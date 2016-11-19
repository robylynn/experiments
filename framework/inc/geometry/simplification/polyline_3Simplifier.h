#ifndef _FRAMEWORK_GEOMETRY_SIMPLIFICATION_POLYLINE_3_SIMPLIFIER_H_
#define _FRAMEWORK_GEOMETRY_SIMPLIFICATION_POLYLINE_3_SIMPLIFIER_H_

#include "geometryTypes.h"
#include "polyline_3.h"

/**
 * The types of simplification strategies used by the Polyline_3 simplification
 * algorithm.
 */
struct Polyline_3SimplificationStrategyDouglasPeucker {};
struct Polyline_3SimplificationStrategyNaiveCircleFit {};

template <typename Polyline_3>
std::tuple<size_t, Polyline_3> naiveCircleSimplify(const Polyline_3& polyline,
                                                   float tolerance);
template <typename Polyline_3>
std::tuple<size_t, Polyline_3> douglasPeuckerSimplify(
    const Polyline_3& polyline, float tolerance);
/**
 * Provides for simplification algorithms for Polyline_3's. The simplification
 * strategy can be used to select the type of simplification algorithm
 * employed.
 */
template <typename Polyline_3>
class Polyline_3Simplifier {
 public:
  Polyline_3Simplifier(float tolerance) : m_tolerance(tolerance) {}

  std::tuple<size_t, Polyline_3> simplify(
      const Polyline_3& polyline,
      const Polyline_3SimplificationStrategyDouglasPeucker&) {
    return douglasPeuckerSimplify(polyline, m_tolerance);
  }

  std::tuple<size_t, Polyline_3> simplify(
      const Polyline_3& polyline,
      const Polyline_3SimplificationStrategyNaiveCircleFit&) {
    return naiveCircleSimplify<Polyline_3>(polyline, m_tolerance);
  }

 private:
  float m_tolerance;
};

#endif  //_FRAMEWORK_GEOMETRY_SIMPLIFICATION_POLYLINE_3_SIMPLIFIER_H_
