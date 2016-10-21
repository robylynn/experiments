#include <gtest/gtest.h>

#include <vector>

#include "uniformPlanarGrid.h"

constexpr int GRID_SIZE = 2;
constexpr float GRID_EXTENT = 2.0;

class UniformPlanarGridTest : public ::testing::Test {
 protected:
  virtual void SetUp() {
    grid1.reset(new UniformPlanarGrid(
        Kernel::Plane_3(planePoints[0], planePoints[1], planePoints[2]),
        GRID_SIZE, GRID_SIZE, GRID_EXTENT, GRID_EXTENT));

    grid2.reset(new UniformPlanarGrid(planePoints[0], planePoints[1],
                                      planePoints[2], GRID_SIZE, GRID_SIZE,
                                      GRID_EXTENT, GRID_EXTENT));
  }

  std::unique_ptr<UniformPlanarGrid> grid1;
  std::unique_ptr<UniformPlanarGrid> grid2;

  std::vector<Kernel::Point_3> expected = {
      Kernel::Point_3(-0.5, -0.5, 0), Kernel::Point_3(0.5, -0.5, 0),
      Kernel::Point_3(-0.5, 0.5, 0), Kernel::Point_3(0.5, 0.5, 0)};
  std::vector<Kernel::Point_3> planePoints = {Kernel::Point_3(0, 0, 0),
                                              Kernel::Point_3(1, 0, 0),
                                              Kernel::Point_3(0, 1, 0)};
};

TEST_F(UniformPlanarGridTest, size) {
  EXPECT_EQ(GRID_SIZE * GRID_SIZE, grid1->size());
  EXPECT_EQ(GRID_SIZE * GRID_SIZE, grid2->size());
}

TEST_F(UniformPlanarGridTest, iterationInitPlane) {
  auto expectedIter = expected.begin();
  size_t count = 0;
  for (const auto& point : *grid1) {
    ASSERT_LT(count++, grid1->size());
    EXPECT_EQ(point, *expectedIter++);
  }
  EXPECT_EQ(count, grid1->size());
}

TEST_F(UniformPlanarGridTest, iterationInitPoints) {
  auto expectedIter = expected.begin();
  size_t count = 0;
  for (const auto& point : *grid2) {
    ASSERT_LT(count++, grid2->size());
    EXPECT_EQ(point, *expectedIter++);
  }
  EXPECT_EQ(count, grid2->size());
}

TEST_F(UniformPlanarGridTest, iterationNonSquarePoints) {
  auto expectedIter = expected.begin();
  UniformPlanarGrid nonSquareGrid(planePoints[0], planePoints[1],
                                  planePoints[2], GRID_SIZE, 1, GRID_EXTENT,
                                  GRID_EXTENT);
  size_t count = 0;
  for (const auto& point : nonSquareGrid) {
    ASSERT_LT(count++, nonSquareGrid.size());
    EXPECT_EQ(point, *expectedIter++ + Kernel::Vector_3(0, 0.5, 0));
  }
  EXPECT_EQ(count, nonSquareGrid.size());
}

TEST_F(UniformPlanarGridTest, iterationUnitGrid) {
  auto expectedIter = expected.begin();
  UniformPlanarGrid unitGrid(planePoints[0], planePoints[1], planePoints[2], 1,
                             1, GRID_EXTENT, GRID_EXTENT);
  size_t count = 0;
  EXPECT_EQ(unitGrid.size(), 1);
  for (const auto& point : unitGrid) {
    ASSERT_LT(count++, unitGrid.size());
    EXPECT_EQ(point, Kernel::Point_3(0, 0, 0));
  }
  EXPECT_EQ(count, 1);
}
