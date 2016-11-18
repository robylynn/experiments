#include <gtest/gtest.h>

#include <polyline.h>
#include <simplification/polyline_3Simplifier.h>

using Polyline_3 = Polyline<Kernel::Point_3>;

TEST(Polyline_3SimplificationTest, circleFitSimple) {
  Polyline_3Simplifier simplifier(0.05);
  Polyline_3 line;
  line.addPoint(Kernel::Point_3(-1, 0, 0));
  line.addPoint(Kernel::Point_3(0, 1, 0));
  line.addPoint(Kernel::Point_3(1, 0, 0));

  auto result =
      simplifier.simplify(line, Polyline_3SimplificationStrategyNaiveBiarc());
  EXPECT_EQ(std::get<0>(result), 1);
}

TEST(Polyline_3SimplificationTest, circleFitCompound) {
  Polyline_3Simplifier simplifier(0.05);
  Polyline_3 line;
  line.addPoint(Kernel::Point_3(-1, 0, 0));
  line.addPoint(Kernel::Point_3(0, 1, 0));
  line.addPoint(Kernel::Point_3(1, 0, 0));
  line.addPoint(Kernel::Point_3(2, 0, 0));

  auto result =
      simplifier.simplify(line, Polyline_3SimplificationStrategyNaiveBiarc());
  EXPECT_EQ(std::get<0>(result), 1);

  line.addPoint(Kernel::Point_3(3, 1, 0));
  line.addPoint(Kernel::Point_3(4, 0, 0));
  line.addPoint(Kernel::Point_3(3, -1, 0));
  line.addPoint(Kernel::Point_3(2, 0, 0));
  result =
      simplifier.simplify(line, Polyline_3SimplificationStrategyNaiveBiarc());
  EXPECT_EQ(std::get<0>(result), 2);
}
