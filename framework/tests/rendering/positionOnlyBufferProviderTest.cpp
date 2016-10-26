#include <gtest/gtest.h>

#include "polyloop.h"
#include "polyloop_2.h"
#include "polyloopGeometryProvider.h"
#include "positionOnlyBufferProvider.h"
#include "vertexElement.h"

class PositionOnlyProviderTest : public ::testing::Test {
 protected:
  virtual void SetUp() {
    loop3D.addPoint(Kernel::Point_3(0, 1, 0));
    loop3D.addPoint(Kernel::Point_3(0, 0, 0));

    loop2D.addPoint(Kernel::Point_2(0, 1));
    loop2D.addPoint(Kernel::Point_2(0, 0));
  }
  virtual void TearDown() {}

  Polyloop<Kernel::Point_3> loop3D;
  Polyloop_2 loop2D;
  float unrolledValues[6] = {0, 1, 0, 0, 0, 0};
};

TEST_F(PositionOnlyProviderTest, iterate) {
  using GeometryProvider = PolyloopGeometryProvider<Polyloop<Kernel::Point_3>>;
  using BufferProvider = PositionOnlyBufferProvider<GeometryProvider>;
  using BufferIter = BufferProvider::const_iterator;
  GeometryProvider geometry(loop3D);
  BufferProvider buffer;
  float* expectedValuePtr = unrolledValues;
  size_t count = 0;
  for (auto iter = buffer.begin(geometry.begin(), PositionVertexElement());
       iter != buffer.end(geometry.end(), PositionVertexElement()); ++iter) {
    ASSERT_LT(count, sizeof(unrolledValues) / sizeof(unrolledValues[0]));
    EXPECT_EQ(*iter, *expectedValuePtr++);
    ++count;
  }
  EXPECT_EQ(count, sizeof(unrolledValues) / sizeof(unrolledValues[0]));
}

TEST_F(PositionOnlyProviderTest, iterate2D) {
  using GeometryProvider = PolyloopGeometryProvider<Polyloop_2>;
  using BufferProvider = PositionOnlyBufferProvider<GeometryProvider>;
  using BufferIter = BufferProvider::const_iterator;
  GeometryProvider geometry(loop2D);
  BufferProvider buffer;
  float* expectedValuePtr = unrolledValues;
  size_t count = 0;
  for (auto iter = buffer.begin(geometry.begin(), PositionVertexElement());
       iter != buffer.end(geometry.end(), PositionVertexElement()); ++iter) {
    ASSERT_LT(count, sizeof(unrolledValues) / sizeof(unrolledValues[0]));
    EXPECT_EQ(*iter, *expectedValuePtr++);
    ++count;
  }
  EXPECT_EQ(count, sizeof(unrolledValues) / sizeof(unrolledValues[0]));
}
