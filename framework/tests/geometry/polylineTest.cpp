#include <gtest/gtest.h>

#include "polyline.h"
#include "polylineGeometryProvider.h"

class PolylineTest : public ::testing::Test {
 protected:
  virtual void SetUp() {
    p.addPoint(Kernel::Point_3(0, 0, 0));
    p.addPoint(Kernel::Point_3(1, 0, 0));
    p.addPoint(Kernel::Point_3(0, 1, 0));
  }
  virtual void TearDown() {}

  Polyline<Kernel::Point_3> p;
};

class PolylineProviderTest : public ::testing::Test {
 protected:
  virtual void SetUp() {
    for (const auto& point : expectedPoints) {
      p.addPoint(point);
    }
  }
  virtual void TearDown() {}

  Polyline<Kernel::Point_3> p;
  std::vector<Kernel::Point_3> expectedPoints{Kernel::Point_3(0, 0, 0),
                                              Kernel::Point_3(1, 0, 0),
                                              Kernel::Point_3(0, 1, 0)};
};

class PolylineLoaderTest : public ::testing::Test {
 protected:
  virtual void SetUp() {}
  virtual void TearDown() {}

  Polyline<Kernel::Point_3> p;
};

TEST_F(PolylineTest, size) { EXPECT_EQ(3, p.size()); }

TEST_F(PolylineTest, next) {
  EXPECT_EQ(p.begin() + 1, p.next(p.begin()));
  EXPECT_EQ(p.end(), p.next(p.begin() + p.size() - 1));
}

TEST_F(PolylineTest, segments) {
  EXPECT_EQ(*p.beginSegment(),
            Kernel::Segment_3(*p.begin(), *p.next(p.begin())));
}

TEST_F(PolylineTest, squaredDistance) {
  EXPECT_EQ(p.squaredDistance(*p.begin()), 0);
  EXPECT_EQ(p.squaredDistance(Kernel::Point_3(0, 0, 1)), 1);
  EXPECT_EQ(p.squaredDistance(Kernel::Point_3(0, 0.5, 0)), 0.125);
  EXPECT_EQ(p.squaredDistance(Kernel::Point_3(2, 0, 0)), 1);
}

TEST_F(PolylineProviderTest, pointTest) {
  PolylineGeometryProvider<decltype(p), PolylinePointPolicy> pointProvider(p);
  int expectedIndex = 0;
  for (auto iter = pointProvider.begin(); iter != pointProvider.end(); ++iter) {
    EXPECT_EQ(*iter, expectedPoints[expectedIndex++]);
  }
  EXPECT_EQ(expectedIndex, expectedPoints.size());
}

TEST_F(PolylineProviderTest, edgeListTest) {
  PolylineGeometryProvider<decltype(p)> listProvider(p);
  int expectedIndex = 0;
  for (auto iter = listProvider.begin(); iter != listProvider.end(); ++iter) {
    EXPECT_EQ(*iter, expectedPoints[expectedIndex / 2 + expectedIndex % 2]);
    expectedIndex++;
  }
  EXPECT_EQ(expectedIndex, 2 * (expectedPoints.size() - 1));
}

TEST_F(PolylineLoaderTest, loading) {
  EXPECT_EQ(buildPolylineFromObj("data/polylineLoad.obj", p), true);
  EXPECT_EQ(p.size(), 96);
}
