#include <gtest/gtest.h>

#include "polyloop_2.h"
#include "simplicialAdaptors/polyloopSimplicialAdaptor.h"

class Polyloop_2RepTest : public ::testing::Test {
 protected:
  virtual void SetUp() {
    p.add(Kernel::Point_2(0, 0));
    p.add(Kernel::Point_2(1, 0));
    p.add(Kernel::Point_2(0, 1));
  }
  virtual void TearDown() {}

  Polyloop_2 p;
};

class Polyloop_2SimplicialAdaptorTest : public ::testing::Test {
 protected:
  virtual void SetUp() {
    p.add(Kernel::Point_2(0, 0));
    p.add(Kernel::Point_2(100, 100));
    p.add(Kernel::Point_2(100, 0));
  }
  virtual void TearDown() {}

  Polyloop_2 p;
};

class Polyloop_2LoaderTest : public ::testing::Test {
 protected:
  virtual void SetUp() {}
  virtual void TearDown() {}

  Polyloop_2 p;
};

TEST_F(Polyloop_2RepTest, size) { EXPECT_EQ(3, p.size()); }

TEST_F(Polyloop_2RepTest, next) {
  EXPECT_EQ(*p.begin(), *p.next(p.begin() + p.size() - 1));
}

TEST_F(Polyloop_2RepTest, segments) {
  EXPECT_EQ(*p.beginSegment(),
            Kernel::Segment_2(*p.begin(), *p.next(p.begin())));
}

TEST_F(Polyloop_2RepTest, squaredDistance) {
  EXPECT_EQ(p.squaredDistance(*p.begin()), 0);
  EXPECT_EQ(p.squaredDistance(Kernel::Point_2(0, 0)), 0);
  EXPECT_EQ(p.squaredDistance(Kernel::Point_2(0, 0.5)), 0);
  EXPECT_EQ(p.squaredDistance(Kernel::Point_2(2, 0)), 1);
}

TEST_F(Polyloop_2SimplicialAdaptorTest, size) {
  PolyloopSimplicialAdaptor<Polyloop_2, LineList> provider(p);
  EXPECT_EQ(2 * p.size(), provider.size());
}

TEST_F(Polyloop_2SimplicialAdaptorTest, iterate) {
  PolyloopSimplicialAdaptor<Polyloop_2, LineList> provider(p);
  auto iter = provider.begin();
  int numElements = 0;
  for (; iter != provider.end(); ++iter) {
    ASSERT_LT(numElements++, provider.size());
  }
  EXPECT_EQ(numElements, provider.size());
}

TEST_F(Polyloop_2LoaderTest, loading) {
  EXPECT_EQ(buildPolyloop_2FromObj("data/polyloop2DLoad.obj", p), true);
  EXPECT_EQ(p.size(), 47);
  EXPECT_EQ(*p.begin(), Kernel::Point_2(-1.0f, 0.0f));
}
