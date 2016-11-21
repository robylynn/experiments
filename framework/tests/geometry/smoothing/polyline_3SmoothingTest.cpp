#include <gtest/gtest.h>

#include <polyline.h>
#include <smoothing/polyline_3Smoother.h>

using Polyline_3 = Polyline<Kernel::Point_3>;

TEST(Polyline_3SmoothingTest, laplacianSmoothingNOP) {
  Polyline_3Smoother smoother;
  Polyline_3 line;

  line.addPoint(Kernel::Point_3(-1, 0, 0));
  line.addPoint(Kernel::Point_3(0, 0, 0));
  line.addPoint(Kernel::Point_3(1, 0, 0));

  smoother.smooth(line, Polyline_3SmoothingStrategyLaplacian());
}
