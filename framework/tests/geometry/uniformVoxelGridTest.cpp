#include <gtest/gtest.h>

#include <memory>

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
