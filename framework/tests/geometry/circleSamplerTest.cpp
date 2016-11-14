#include <gtest/gtest.h>

#include <CGAL/squared_distance_3.h>

#include "samplers/orderedCircleSampler.h"
#include "CGALUtils.h"
#include "geometryTypes.h"

constexpr double NORMALIZATION_ERROR = 0.000000000001;

class OrderedPointsOnCircleGeneratorTest : public ::testing::Test {
 protected:
  virtual void SetUp() {}
};

TEST_F(OrderedPointsOnCircleGeneratorTest, canonicalSampleTest) {
  OrderedPointsOnCircleGenerator sampler(
      Kernel::Circle_3(
          Kernel::Point_3(0, 0, 0), 1,
          Kernel::Plane_3(Kernel::Point_3(0, 0, 0), Kernel::Vector_3(0, 0, 1))),
      4);
  std::vector<Kernel::Point_3> expected{
      Kernel::Point_3(1, 0, 0), Kernel::Point_3(0, 1, 0),
      Kernel::Point_3(-1, 0, 0), Kernel::Point_3(0, -1, 0)};

  auto iter1 = expected.begin();
  auto iter2 = sampler.begin();
  for (; iter1 != expected.end(); ++iter1, ++iter2) {
    ASSERT_TRUE(iter2 != sampler.end());
    EXPECT_NEAR(CGAL::squared_distance(*iter1, *iter2), 0, NORMALIZATION_ERROR);
  }
  ASSERT_TRUE(iter2 == sampler.end());
}

TEST_F(OrderedPointsOnCircleGeneratorTest, xzSampleTest) {
  OrderedPointsOnCircleGenerator sampler(
      Kernel::Circle_3(
          Kernel::Point_3(0, 0, 0), 1,
          Kernel::Plane_3(Kernel::Point_3(0, 0, 0), Kernel::Vector_3(0, 1, 0))),
      4);
  // We take this dependency on ordering of a plane's basis here, but it is not
  // guaranteed. We may want to know if this changes.
  std::vector<Kernel::Point_3> expected{
      Kernel::Point_3(1, 0, 0), Kernel::Point_3(0, 0, -1),
      Kernel::Point_3(-1, 0, 0), Kernel::Point_3(0, 0, 1)};

  auto iter1 = expected.begin();
  auto iter2 = sampler.begin();
  for (; iter1 != expected.end(); ++iter1, ++iter2) {
    ASSERT_TRUE(iter2 != sampler.end());
    EXPECT_NEAR(CGAL::squared_distance(*iter1, *iter2), 0, NORMALIZATION_ERROR);
  }
  ASSERT_TRUE(iter2 == sampler.end());
}
