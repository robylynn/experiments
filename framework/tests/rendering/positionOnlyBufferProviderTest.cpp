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
  float unrolledValues[6] = {0, 1, 0, 0, 0, 0};
};

TEST_F(PositionOnlyProviderTest, iterate) {
  using GeometryProvider = PolyloopGeometryProvider<Kernel::Point_3>;
  using BufferProvider = PositionOnlyBufferProvider<GeometryProvider>;
  using BufferIter = BufferProvider::const_iterator;
  GeometryProvider geometry(loop);
  BufferProvider buffer;
  float* expectedValuePtr = unrolledValues;
  size_t count = 0;
  for (auto iter = buffer.begin(geometry.begin(), PositionVertexElement());
       iter != buffer.end(geometry.end(), PositionVertexElement()); ++iter) {
    ASSERT_LT(count, sizeof(unrolledValues) / sizeof(unrolledValues[0]));
    EXPECT_EQ(*iter, *expectedValuePtr++);
    ++count;
  }
}
