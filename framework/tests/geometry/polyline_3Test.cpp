#include <gtest/gtest.h>

#include "polyline_3.h"

class Polyline_3Test : public ::testing::Test {
 protected:
  virtual void SetUp() {
    p.add(Kernel::Point_3(0, 0, 0));
    p.add(Kernel::Point_3(1, 0, 0));
    p.add(Kernel::Point_3(0, 1, 0));
  }
  virtual void TearDown() {}

  GeometryPolyline_3 p;
};

class Polyline_3LoaderTest : public ::testing::Test {
 protected:
  virtual void SetUp() {}
  virtual void TearDown() {}

  GeometryPolyline_3 p;
};

TEST_F(Polyline_3Test, size) { EXPECT_EQ(3, p.size()); }

/*TEST_F(PolylineTest, segments) {
  EXPECT_EQ(*p.beginSegment(),
            Kernel::Segment_3(*p.begin(), *p.next(p.begin())));
}

TEST_F(PolylineTest, squaredDistance) {
  EXPECT_EQ(p.squaredDistance(*p.begin()), 0);
  EXPECT_EQ(p.squaredDistance(Kernel::Point_3(0, 0, 1)), 1);
  EXPECT_EQ(p.squaredDistance(Kernel::Point_3(0, 0.5, 0)), 0.125);
  EXPECT_EQ(p.squaredDistance(Kernel::Point_3(2, 0, 0)), 1);
}*/

TEST_F(Polyline_3LoaderTest, loading) {
  EXPECT_EQ(buildPolyline_3FromObj("data/polylineLoad.obj", p), true);
  EXPECT_EQ(p.size(), 96);
}
