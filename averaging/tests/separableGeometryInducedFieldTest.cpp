#include <gtest/gtest.h>

#include "geometryVariants.h"
#include "polyloop.h"
#include "separableGeometryInducedField.h"
#include "distanceFieldComputers.h"

class DistanceFieldTest : public ::testing::Test {
 protected:
  virtual void SetUp() {}
  SeparableGeometryInducedField<PointDistanceComputableTypes,
                                SquaredDistanceFieldComputer> m_field;
};

TEST_F(DistanceFieldTest, queryFieldValueTest) {
  Polyloop<Kernel::Point_3> p;
  p.addPoint(Kernel::Point_3(0, 0, 0));
  p.addPoint(Kernel::Point_3(1, 0, 0));
  p.addPoint(Kernel::Point_3(0, 1, 0));
  m_field.addGeometry(p);
  //EXPECT_FLOAT_EQ(m_field.pointSample(*(p.begin())), 0);
  //EXPECT_FLOAT_EQ(m_field.pointSample(Kernel::Point_3(0, 0, 1)), 0);
}
