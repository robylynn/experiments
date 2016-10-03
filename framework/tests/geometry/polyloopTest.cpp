#include <gtest/gtest.h>

#include "polyloop.h"
#include "polyloopGeometryProvider.h"

class PolyloopTest : public ::testing::Test {
 protected:
  virtual void SetUp() {
    p.addPoint(CGAL::Point_3<Kernel>(0, 0, 0));
    p.addPoint(CGAL::Point_3<Kernel>(100, 100, -100));
    p.addPoint(CGAL::Point_3<Kernel>(100, 0, -100));
  }
  virtual void TearDown() {}

  Polyloop<Kernel::Point_3> p;
};

class PolyloopGeometryProviderTest : public ::testing::Test {
 protected:
  virtual void SetUp() {
    p.addPoint(CGAL::Point_3<Kernel>(0, 0, 0));
    p.addPoint(CGAL::Point_3<Kernel>(100, 100, -100));
    p.addPoint(CGAL::Point_3<Kernel>(100, 0, -100));
  }
  virtual void TearDown() {}

  Polyloop<Kernel::Point_3> p;
};

TEST_F(PolyloopTest, size) {
  EXPECT_EQ(3, p.size());
}

TEST_F(PolyloopGeometryProviderTest, size) {
  PolyloopGeometryProvider<Kernel::Point_3> provider(p);
  EXPECT_EQ(4, provider.size());
}

TEST_F(PolyloopGeometryProviderTest, iterate) {
  PolyloopGeometryProvider<Kernel::Point_3> provider(p);
  auto iter = provider.begin();
  for (;iter != provider.end(); ++iter);
  EXPECT_EQ(*iter, *p.begin());
}
