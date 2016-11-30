#ifndef _AVERAGING_COMMON_VIEW_INTERFACE_H_
#define _AVERAGING_COMMON_VIEW_INTERFACE_H_

#include "distanceFieldComputers.h"
#include "separableGeometryInducedField.h"

using SquaredDistField_3 =
    SeparableGeometryInducedField<Kernel::Point_3,
                                  SquaredDistanceFieldComputer>;

using SquaredDistField_2 =
    SeparableGeometryInducedField<Kernel::Point_2,
                                  SquaredDistanceFieldComputer>;

// Common interface implemented by all averaging views
class IAveragingView {
 public:
  virtual ~IAveragingView(){};
};

#endif  //_AVERAGING_COMMON_VIEW_INTERFACE_H_
