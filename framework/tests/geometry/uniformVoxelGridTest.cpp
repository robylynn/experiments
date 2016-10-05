#include <gtest/gtest.h>

#include <memory>

#include "containerAlgorithms.h"
#include "uniformVoxelGrid.h"
#include "uniformVoxelGridGeometryProvider.h"

constexpr int GRID_SIZE = 10;
constexpr float GRID_EXTENT = 10.0;

class UniformVoxelGridTest : public ::testing::Test {
 protected:
  virtual void SetUp() {
    grid.reset(new UniformVoxelGrid(GRID_EXTENT, GRID_SIZE));
  }

  virtual void TearDown() {}

  std::unique_ptr<UniformVoxelGrid> grid;
};

class UniformVoxelGridGeometryProviderTest : public ::testing::Test {
 protected:
  virtual void SetUp() {
    grid.reset(new UniformVoxelGrid(GRID_EXTENT, GRID_SIZE));
  }

  virtual void TearDown() {}

  std::unique_ptr<UniformVoxelGrid> grid;
};

TEST_F(UniformVoxelGridTest, size) {
  EXPECT_EQ(GRID_SIZE * GRID_SIZE * GRID_SIZE, grid->size());
}

TEST_F(UniformVoxelGridTest, voxelBounds) {
  EXPECT_EQ(-GRID_EXTENT, grid->voxelBoundsForLocation(*grid->begin())[0].x());
  // TODO msati3: What is the ordering of generated grid points? Figuring out
  // this will enable writing additional tests for the voxel grid
  /*EXPECT_EQ(-GRID_EXTENT + (2 * GRID_EXTENT / GRID_SIZE),
            grid->voxelBoundsForLocation(*(++grid->begin()))[0].x());*/
}

TEST_F(UniformVoxelGridGeometryProviderTest, pointProviderIteration) {
  VoxelGridPointProvider pointProvider(*grid);
  size_t numPoints =
      utils::count_all(pointProvider.begin(), pointProvider.end());
  // The point provider creates one point per voxel grid point.
  EXPECT_EQ(grid->size(), pointProvider.size());
  EXPECT_EQ(pointProvider.size(), numPoints);
}

TEST_F(UniformVoxelGridGeometryProviderTest, pointProviderContent) {
  VoxelGridPointProvider pointProvider(*grid);
  auto vertexIter = pointProvider.begin();
  EXPECT_EQ(vertexIter->x(), -GRID_EXTENT);
  EXPECT_EQ(vertexIter->y(), -GRID_EXTENT);
  EXPECT_EQ(vertexIter->z(), -GRID_EXTENT);
}

TEST_F(UniformVoxelGridGeometryProviderTest, cubeProviderIteration) {
  VoxelGridCubeProvider cubeProvider(*grid);
  size_t numPoints = utils::count_all(cubeProvider.begin(), cubeProvider.end());
  // The cube provider creates TRIANGLES_PER_CUBOID * 3 points per voxel grid
  // point
  EXPECT_EQ(grid->size() * AlignedCuboidTrianglesBuilder::TRIANGLES_PER_CUBOID *
                VERTICES_PER_TRIANGLE,
            cubeProvider.size());
  EXPECT_EQ(cubeProvider.size(), numPoints);
}

TEST_F(UniformVoxelGridGeometryProviderTest, cubeProviderContent) {
  VoxelGridCubeProvider cubeProvider(*grid);
  auto vertexIter = cubeProvider.begin();
  EXPECT_EQ(vertexIter->x(), -GRID_EXTENT);
  EXPECT_EQ(vertexIter->y(), -GRID_EXTENT);
  EXPECT_EQ(vertexIter->z(), -GRID_EXTENT);
}
