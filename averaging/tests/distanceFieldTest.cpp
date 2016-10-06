#include <gtest/gtest.h>

#include "squaredDistanceField.h"
#include "polyloop.h"

class DistanceFieldTest : public ::testing::Test {
 protected:
  virtual void SetUp() {
    field.reset(new GeometryInducedDistanceField<UniformVoxelGrid>(10.0, 1));
  }

  std::unique_ptr<GeometryInducedDistanceField<UniformVoxelGrid>> field;
};

TEST_F(DistanceFieldTest, queryFieldValueTest) {
  Polyloop<Kernel::Point_3> p;
  p.addPoint(Kernel::Point_3(0, 0, 0));
  p.addPoint(Kernel::Point_3(1, 0, 0));
  p.addPoint(Kernel::Point_3(0, 1, 0));
  field->addPolyloop(p);
  EXPECT_EQ(field->queryDistanceField(*p.begin()), 1);
}
