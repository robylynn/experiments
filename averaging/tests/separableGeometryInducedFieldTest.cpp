#include <gtest/gtest.h>

#include "geometryVariants.h"
#include "polyloop.h"
#include "squaredDistance.h"

#include "separableGeometryInducedField.h"
#include "distanceFieldComputers.h"
#include "fieldSampler.h"
#include "uniformPlanarGrid.h"
#include "gradientComputer.h"

class DistanceFieldTest : public ::testing::Test {
 protected:
  virtual void SetUp() {
    p.addPoint(Kernel::Point_3(0.5, 0, 0));
    p.addPoint(Kernel::Point_3(1, 0, 0));
    p.addPoint(Kernel::Point_3(0.5, 1, 0));
    field.addGeometry(p);
  }
  using Field = SeparableGeometryInducedField <
                SquaredDistanceFieldComputer<Kernel::Point_3>>;
  Field field;
  Polyloop<Kernel::Point_3> p;
};

TEST_F(DistanceFieldTest, queryFieldValueTest) {
  EXPECT_FLOAT_EQ(field(*p.begin()), 0);
  EXPECT_FLOAT_EQ(field(Kernel::Point_3(0, 0, 1)), 1.25);
}

// Create a 1X1 grid from (-1,-1,0) - (1,1,0) and query at (0,0,0)
TEST_F(DistanceFieldTest, fieldSamplerTest) {
  UniformPlanarGrid grid(Kernel::Point_3(0, 0, 0), Kernel::Point_3(1, 0, 0),
                         Kernel::Point_3(0, 1, 0), 1, 1, 1.0, 1.0);
  FieldSampler<Field, UniformPlanarGrid::const_iterator> sampler(
      std::bind(&Field::operator(), &field, std::placeholders::_1),
      grid.begin(), grid.end());

  size_t numSamples = 0;
  for (auto tuple : sampler) {
    ASSERT_LT(numSamples, 1);
    EXPECT_FLOAT_EQ(std::get<1>(tuple), 0.25);
    ++numSamples;
  }
  EXPECT_EQ(numSamples, 1);
}

TEST_F(DistanceFieldTest, gradientEstimationTest) {
  NaiveGradientEstimator estimator(0.1);
  GradientComputer<Field> gradientComputer(field, estimator);
  EXPECT_EQ(gradientComputer(Kernel::Point_3(0, 0.5, 0)).y(), 0);
  EXPECT_FLOAT_EQ(gradientComputer(Kernel::Point_3(0, 0.5, 0)).x(),
                  0.4 * 0.4 - 0.5 * 0.5);
}
