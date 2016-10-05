#include <gtest/gtest.h>

#include <memory>

#include "containerAlgorithms.h"
#include "uniformVoxelGrid.h"
#include "uniformVoxelGridGeometryProvider.h"

class UniformVoxelGridTest : public ::testing::Test {
 protected:
  virtual void SetUp() { grid.reset(new UniformVoxelGrid(10, 10)); }

  virtual void TearDown() {}

  std::unique_ptr<UniformVoxelGrid> grid;
};

class UniformVoxelGridGeometryProviderTest : public ::testing::Test {
 protected:
  virtual void SetUp() { grid.reset(new UniformVoxelGrid(10, 10)); }

  virtual void TearDown() {}

  std::unique_ptr<UniformVoxelGrid> grid;
};

TEST_F(UniformVoxelGridTest, size) { EXPECT_EQ(10 * 10 * 10, grid->size()); }

TEST_F(UniformVoxelGridGeometryProviderTest, pointProviderIteration) {
  VoxelGridPointProvider pointProvider(*grid);
  size_t numPoints =
      utils::count_all(pointProvider.begin(), pointProvider.end());
  EXPECT_EQ(pointProvider.size(), numPoints);
}

/*TEST_F(UniformVoxelGridGeometryProviderTest, cubeProviderIteration) {
  VoxelGridCubeProvider cubeProvider(*grid);
  size_t numPoints = utils::count_all(cubeProvider.begin(), cubeProvider.end());
  EXPECT_EQ(cubeProvider.size() *
                AlignedCuboidTrianglesBuilder::TRIANGLES_PER_CUBOID *
                VERTICES_PER_TRIANGLE,
            numPoints);
}*/
