#include <gtest/gtest.h>

#include "polyloop_3.h"
#include "polyloopGeometryProvider.h"

class PolyloopTest : public ::testing::Test {
 protected:
  virtual void SetUp() {
    p.addPoint(Kernel::Point_3(0, 0, 0));
    p.addPoint(Kernel::Point_3(1, 0, 0));
    p.addPoint(Kernel::Point_3(0, 1, 0));
  }
  virtual void TearDown() {}

  Polyloop_3 p;
};

class PolyloopGeometryProviderTest : public ::testing::Test {
 protected:
  virtual void SetUp() {
    p.addPoint(Kernel::Point_3(0, 0, 0));
    p.addPoint(Kernel::Point_3(100, 100, -100));
    p.addPoint(Kernel::Point_3(100, 0, -100));
  }
  virtual void TearDown() {}

  Polyloop_3 p;
};

class PolyloopLoaderTest : public ::testing::Test {
 protected:
  virtual void SetUp() {}
  virtual void TearDown() {}

  Polyloop_3 p;
};

TEST_F(PolyloopTest, size) { EXPECT_EQ(3, p.size()); }

TEST_F(PolyloopTest, next) {
  EXPECT_EQ(*p.begin(), *p.next(p.begin() + p.size() - 1));
}

TEST_F(PolyloopTest, segments) {
  EXPECT_EQ(*p.beginSegment(),
            Kernel::Segment_3(*p.begin(), *p.next(p.begin())));
}

TEST_F(PolyloopTest, squaredDistance) {
  EXPECT_EQ(p.squaredDistance(*p.begin()), 0);
  EXPECT_EQ(p.squaredDistance(Kernel::Point_3(0, 0, 1)), 1);
  EXPECT_EQ(p.squaredDistance(Kernel::Point_3(0, 0.5, 0)), 0);
  EXPECT_EQ(p.squaredDistance(Kernel::Point_3(2, 0, 0)), 1);
}

TEST_F(PolyloopGeometryProviderTest, size) {
  PolyloopGeometryProvider<Polyloop_3> provider(p);
  EXPECT_EQ(4, provider.size());
}

TEST_F(PolyloopGeometryProviderTest, iterate) {
  PolyloopGeometryProvider<Polyloop_3> provider(p);
  auto iter = provider.begin();
  for (; iter != provider.end(); ++iter) {
  }
  EXPECT_EQ(*iter, *p.begin());
}

TEST_F(PolyloopLoaderTest, loading) {
  EXPECT_EQ(buildPolyloopFromObj("data/polyloopLoad.obj", p), true);
  EXPECT_EQ(p.size(), 95);
}

