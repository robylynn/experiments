#include <gtest/gtest.h>

#include "polyloop.h"
#include "polyloopGeometryProvider.h"
#include "vertexElement.h"
#include "positionOnlyProvider.h"

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
  PositionOnlyProvider<PolyloopGeometryProvider<Kernel::Point_3>>
      positionProvider(loopProvider);
  EXPECT_EQ(*(++positionProvider.begin(PositionVertexElement())), 1);
}
