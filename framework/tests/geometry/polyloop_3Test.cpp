#include <gtest/gtest.h>

#include "polyloop_3.h"
#include "simplicialAdaptors/polyloopSimplicialAdaptor.h"

class PolyloopTest : public ::testing::Test {
 protected:
  virtual void SetUp() {
    p.add(Kernel::Point_3(0, 0, 0));
    p.add(Kernel::Point_3(1, 0, 0));
    p.add(Kernel::Point_3(0, 1, 0));
  }
  virtual void TearDown() {}

  GeometryPolyloop_3 p;
};

class PolyloopSimplicialAdaptorTest : public ::testing::Test {
 protected:
  virtual void SetUp() {
    p.add(Kernel::Point_3(0, 0, 0));
    p.add(Kernel::Point_3(1, 1, -1));

    unrolledStripPolicy = {*p.begin(), *(p.begin() + 1), *p.begin()};
    unrolledListPolicy = {*p.begin(), *(p.begin() + 1), *(p.begin() + 1),
                          *p.begin()};
    unrolledMeshPolicy = {*p.begin(),       *(p.begin() + 1), *p.begin(),
                          *(p.begin() + 1), *p.begin(),       *(p.begin() +
    1)};
  }
  virtual void TearDown() {}

  GeometryPolyloop_3 p;
  std::vector<Kernel::Point_3> unrolledStripPolicy;
  std::vector<Kernel::Point_3> unrolledListPolicy;
  std::vector<Kernel::Point_3> unrolledMeshPolicy;
};

class PolyloopLoaderTest : public ::testing::Test {
 protected:
  virtual void SetUp() {}
  virtual void TearDown() {}

  GeometryPolyloop_3 p;
};

TEST_F(PolyloopTest, size) { EXPECT_EQ(3, p.size()); }

TEST_F(PolyloopTest, nextVertex) {
  EXPECT_EQ(*p.begin(), *p.next_vertex(p.begin() + p.size() - 1));
}

/*TEST_F(PolyloopTest, segments) {
  EXPECT_EQ(*p.edges_begin(),
            Kernel::Segment_3(*p.begin(), *p.next_vertex(p.begin())));
}

TEST_F(PolyloopTest, squaredDistance) {
  EXPECT_EQ(p.squaredDistance(*p.begin()), 0);
  EXPECT_EQ(p.squaredDistance(Kernel::Point_3(0, 0, 1)), 1);
  EXPECT_EQ(p.squaredDistance(Kernel::Point_3(0, 0.5, 0)), 0);
  EXPECT_EQ(p.squaredDistance(Kernel::Point_3(2, 0, 0)), 1);
}*/

/*TEST_F(PolyloopSimplicialAdaptorTest, size) {
  PolyloopSimplicialAdaptor<GeometryPolyloop_3, LineList> provider(p);
  EXPECT_EQ(2 * p.size(), provider.size());
}

TEST_F(PolyloopSimplicialAdaptorTest, iterateListPolicy) {
  PolyloopSimplicialAdaptor<GeometryPolyloop_3, LineList> provider(p);
  auto iter = provider.begin();
  auto iterUnrolled = unrolledListPolicy.begin();
  for (; iterUnrolled != unrolledListPolicy.end(); ++iter, ++iterUnrolled) {
    EXPECT_EQ(*iter, *iterUnrolled);
  }
  EXPECT_EQ(iter, provider.end());
}*/

TEST_F(PolyloopLoaderTest, loading) {
  EXPECT_EQ(buildPolyloop_3FromObj("data/polyloopLoad.obj", p), true);
  EXPECT_EQ(p.size(), 95);
}