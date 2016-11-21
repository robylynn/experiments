#ifndef _FRAMEWORK_GEOMETRY_SMOOTHING_POLYLINE_3_SMOOTHER_H_
#define _FRAMEWORK_GEOMETRY_SMOOTHING_POLYLINE_3_SMOOTHER_H_

#include "polyline.h"
#include "laplacianSmoothing.h"

struct Polyline_3SmoothingStrategyLaplacian {};

class Polyline_3Smoother {
 public:
  Polyline<Kernel::Point_3> smooth(const Polyline<Kernel::Point_3>& polyline,
                                   const Polyline_3SmoothingStrategyLaplacian&) {
    return laplacianSmoothing(polyline);
  }
};

#endif  // _FRAMEWORK_GEOMETRY_SMOOTHING_POLYLINE_3_SMOOTHER_H_
