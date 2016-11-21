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

  Polyline_3 smoothLine =
      smoother.smooth(line, Polyline_3SmoothingStrategyLaplacian(0.05, 10));

  auto iter2 = smoothLine.begin();
  for (auto iter1 = line.begin(); iter1 != line.end(); ++iter1, ++iter2) {
    EXPECT_EQ(*iter1, *iter2);
  }
  EXPECT_EQ(smoothLine.end(), iter2);
}

TEST(Polyline_3SmoothingTest, laplacianSmoothingSimple) {
  Polyline_3Smoother smoother;
  Polyline_3 line;

  line.addPoint(Kernel::Point_3(-1, 0, 0));
  line.addPoint(Kernel::Point_3(0, 1, 0));
  line.addPoint(Kernel::Point_3(1, 0, 0));

  Polyline_3 smoothLine =
      smoother.smooth(line, Polyline_3SmoothingStrategyLaplacian(0.05, 100));
  // The smoothing should bring second point near the origin
  EXPECT_NEAR(CGAL::squared_distance(Kernel::Point_3(0, 0, 0),
                                     *(smoothLine.begin() + 1)),
              0, 0.1);
}
