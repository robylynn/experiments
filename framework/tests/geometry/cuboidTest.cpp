#include <gtest/gtest.h>

#include "containerAlgorithms.h"
#include "cuboidGeometryProvider.h"

class CuboidProviderTest : public ::testing::Test {
 protected:
  virtual void SetUp() {
    trianglesBuilder.reset(new AlignedCuboidTrianglesBuilder(
        Kernel::Iso_cuboid_3(0, 0, 0, 10, 10, 10)));
  }

  virtual void TearDown() {}

  std::unique_ptr<AlignedCuboidTrianglesBuilder> trianglesBuilder;
};

TEST_F(CuboidProviderTest, triangleProviderIteration) {
  size_t numTriangles =
      utils::count_all(trianglesBuilder->begin(), trianglesBuilder->end());
  EXPECT_EQ(numTriangles, AlignedCuboidTrianglesBuilder::TRIANGLES_PER_CUBOID);
}
