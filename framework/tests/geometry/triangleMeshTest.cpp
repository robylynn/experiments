#include <gtest/gtest.h>

#include <CGAL/Polyhedron_3.h>
#include <CGAL/Projection_traits_xy_3.h>
#include <CGAL/Triangulation_2.h>

#include "geometryTypes.h"
#include "triangleMeshGeometryProvider.h"

class MeshProviderTest : public ::testing::Test {
 protected:
  virtual void SetUp() {
    tetrahedron.make_tetrahedron(points[0], points[1], points[2], points[3]);
  }

  virtual void TearDown() {}

  CGAL::Polyhedron_3<Kernel> tetrahedron;
  int unrolledIndices[3 * 4] = {0, 1, 2, 3, 0, 2, 3, 1, 0, 3, 2, 1};
  Kernel::Point_3 points[4] = {
      Kernel::Point_3(0, 0, 0), Kernel::Point_3(1, 0, 0),
      Kernel::Point_3(0, 1, 0), Kernel::Point_3(0, 0, 1)};
  using MeshRepPoly = CGAL::Polyhedron_3<Kernel>;
  using MeshRepTriangulation2 =
      CGAL::Triangulation_2<CGAL::Projection_traits_xy_3<Kernel>>;
};

TEST_F(MeshProviderTest, size) {
  TriangleMeshGeometryProvider<MeshRepPoly> provider(tetrahedron);
  EXPECT_EQ(4 * 3, provider.size());
}

TEST_F(MeshProviderTest, iterationPolyhedron) {
  TriangleMeshGeometryProvider<MeshRepPoly> provider(tetrahedron);
  size_t count = 0;
  for (auto iter = provider.begin(); iter != provider.end(); ++iter, ++count) {
    ASSERT_LT(count, provider.size());
    EXPECT_EQ(*iter, points[unrolledIndices[count]]);
  }
}

TEST_F(MeshProviderTest, iterationTriangulation2) {
  CGAL::Triangulation_2<CGAL::Projection_traits_xy_3<Kernel>> triangulation;
  triangulation.insert(&points[0], &points[3]);
  TriangleMeshGeometryProvider<MeshRepTriangulation2> provider(triangulation);
  size_t count = 0;
  for (auto iter = provider.begin(); iter != provider.end(); ++iter, ++count) {
    ASSERT_LT(count, provider.size());
  }
  EXPECT_EQ(count, provider.size());
  std::set<Kernel::Point_3> triangulationPoints(provider.begin(), provider.end());
  std::set<Kernel::Point_3> expectedPoints(&points[0], &points[3]);
  EXPECT_EQ(triangulationPoints, expectedPoints);
}
