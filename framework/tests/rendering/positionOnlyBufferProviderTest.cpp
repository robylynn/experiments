#include <gtest/gtest.h>

#include "polyloop_3.h"
#include "polyloop_2.h"
#include "polyloopGeometryProvider.h"
#include "singleElementBufferProvider.h"

class PositionOnlyProviderTest : public ::testing::Test {
 protected:
  virtual void SetUp() {
    loop3D.addPoint(Kernel::Point_3(0, 1, 0));
    loop3D.addPoint(Kernel::Point_3(0, 0, 0));

    loop2D.addPoint(Kernel::Point_2(0, 1));
    loop2D.addPoint(Kernel::Point_2(0, 0));
  }
  virtual void TearDown() {}

  Polyloop_3 loop3D;
  Polyloop_2 loop2D;
  float unrolledValues[12] = {0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0};
};

TEST_F(PositionOnlyProviderTest, iterateSimple) {
  using GeometryProvider = PolyloopGeometryProvider<Polyloop_3>;
  std::vector<Kernel::Point_3> points = {Kernel::Point_3(0, 0, 0),
                                         Kernel::Point_3(0, 1, 0)};
  GeometryProvider geometry(loop3D);
  BufferProvider buffer();

  float* expectedValuePtr = unrolledValues;
  size_t count = 0;
  for (auto iter = buffer.begin(); iter != buffer.end(); ++iter) {
    ASSERT_LT(count, sizeof(unrolledValues) / sizeof(unrolledValues[0]));
    EXPECT_EQ(*iter, *expectedValuePtr++);
    ++count;
  }
  EXPECT_EQ(count, sizeof(unrolledValues) / sizeof(unrolledValues[0]));
}

TEST_F(PositionOnlyProviderTest, iterate) {
  using GeometryProvider = PolyloopGeometryProvider<Polyloop_3>;
  using BufferProvider = PositionOnlyBufferProvider<GeometryProvider>;
  using BufferIter = BufferProvider::const_iterator;
  GeometryProvider geometry(loop3D);
  BufferProvider buffer(geometry);

  float* expectedValuePtr = unrolledValues;
  size_t count = 0;
  for (auto iter = buffer.begin(); iter != buffer.end(); ++iter) {
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
  BufferProvider buffer(geometry);

  float* expectedValuePtr = unrolledValues;
  size_t count = 0;
  for (auto iter = buffer.begin(); iter != buffer.end(); ++iter) {
    ASSERT_LT(count, sizeof(unrolledValues) / sizeof(unrolledValues[0]));
    EXPECT_EQ(*iter, *expectedValuePtr++);
    ++count;
  }
  EXPECT_EQ(count, sizeof(unrolledValues) / sizeof(unrolledValues[0]));
}
