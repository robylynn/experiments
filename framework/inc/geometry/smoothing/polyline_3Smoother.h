#ifndef _FRAMEWORK_GEOMETRY_SMOOTHING_POLYLINE_3_SMOOTHER_H_
#define _FRAMEWORK_GEOMETRY_SMOOTHING_POLYLINE_3_SMOOTHER_H_

#include "polyline.h"
#include "laplacianSmoothing.h"

struct Polyline_3SmoothingStrategyLaplacian {
  Polyline_3SmoothingStrategyLaplacian(float stepSize, size_t numIterations)
      : m_stepSize(stepSize), m_numIterations(numIterations){};
  float m_stepSize;
  size_t m_numIterations;
};

class Polyline_3Smoother {
 public:
  Polyline<Kernel::Point_3> smooth(
      const Polyline<Kernel::Point_3>& polyline,
      const Polyline_3SmoothingStrategyLaplacian& strategy) {
    return laplacianSmoothing(polyline, strategy.m_stepSize,
                              strategy.m_numIterations);
  }
};

#endif  // _FRAMEWORK_GEOMETRY_SMOOTHING_POLYLINE_3_SMOOTHER_H_
