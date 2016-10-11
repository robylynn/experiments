#include <gtest/gtest.h>

#include "polyloop.h"
#include "polyloopGeometryProvider.h"
#include "positionOnlyBufferProvider.h"
#include "vertexElement.h"

class PositionOnlyProviderTest : public ::testing::Test {
 protected:
  virtual void SetUp() {
    loop.addPoint(Kernel::Point_3(0, 1, 0));
    loop.addPoint(Kernel::Point_3(0, 0, 0));
  }
  virtual void TearDown() {}

  Polyloop<Kernel::Point_3> loop;
};

TEST_F(PositionOnlyProviderTest, iterate) {
  PolyloopGeometryProvider<Kernel::Point_3> loopProvider(loop);
  PositionOnlyBufferProvider<PolyloopGeometryProvider<Kernel::Point_3>>
      positionProvider;
  EXPECT_EQ(
      *++positionProvider.begin(loopProvider.begin(), PositionVertexElement()),
      1);
}
