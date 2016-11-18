#ifndef _POLYLINE_3_SIMPLIFIER_H_
#define _POLYLINE_3_SIMPLIFIER_H_

#include "geometryTypes.h"
#include "polyline.h"

struct Polyline_3SimplificationStrategyDouglasPeucker {};
struct Polyline_3SimplificationStrategyNaiveBiarc {};

class Polyline_3Simplifier {
 public:
  Polyline_3Simplifier(float tolerance) : m_tolerance(tolerance) {}

  std::tuple<size_t, Polyline<Kernel::Point_3>> simplify(
      const Polyline<Kernel::Point_3>& polyline,
      const Polyline_3SimplificationStrategyDouglasPeucker&);

  std::tuple<size_t, Polyline<Kernel::Point_3>> simplify(
      const Polyline<Kernel::Point_3>& polyline,
      const Polyline_3SimplificationStrategyNaiveBiarc&);

 private:
  float m_tolerance;
};

#endif  //_POLYLINE_3_SIMPLIFIER_H_
