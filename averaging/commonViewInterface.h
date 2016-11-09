#ifndef _AVERAGING_COMMON_VIEW_INTERFACE_H_
#define _AVERAGING_COMMON_VIEW_INTERFACE_H_

#include "separableGeometryInducedField.h"
#include "distanceFieldComputers.h"

using SquaredDistField =
    SeparableGeometryInducedField<Kernel::Point_3,
                                  SquaredDistanceFieldComputer>;

// Common interface implemented by all averaging views
class IAveragingView {
 public:
  virtual ~IAveragingView() {};
  virtual const SquaredDistField* getGeometryInducedField() = 0;
};

#endif //_AVERAGING_COMMON_VIEW_INTERFACE_H_
