#ifndef _POLYLINE_SIMPLIFIER_H_
#define _POLYLINE_SIMPLIFIER_H_

#include "geometryTypes.h"
#include "polyline.h"
#include "biarc_3.h"

namespace Polyline_3Simplification {

enum Strategy {
  DOUGLAS_PEUCKER,
  NAIVE_BIARC_FIT,
};

// Most simplification strategies will simplify a polyline to yield a
// representation that is also a polyline.
template <int Strategy>
struct StrategyTraits {
  using result_type = Polyline<Kernel::Point_3>;
};

template <>
struct StrategyTraits<NAIVE_BIARC_FIT> {
  using result_type = Biarc_3;
};

class Simplifier {
 public:
  Simplifier(float tolerance) : m_tolerance(tolerance) {}

  template <int Strategy>
  typename StrategyTraits<Strategy>::result_type simplify(
      const Polyline<Kernel::Point_3>& polyline);

 private:
  float m_tolerance;
};

}  // end of namespace Polyline_3Simplification

#endif  //_POLYLINE_SIMPLIFIER_H_
