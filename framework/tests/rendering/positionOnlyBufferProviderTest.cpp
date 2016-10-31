#include <gtest/gtest.h>

#include "defaultBufferProviders.h"
#include "defaultGeometryProviders.h"

class NoCopyContainer {
 private:
  std::vector<Kernel::Point_3> m_points = {Kernel::Point_3(0, 0, 0),
                                           Kernel::Point_3(0, 1, 0)};

 public:
  NoCopyContainer() {}
  NoCopyContainer(const NoCopyContainer&) = delete;
  NoCopyContainer& operator=(const NoCopyContainer&) = delete;
  NoCopyContainer(const NoCopyContainer&&) = delete;

  using const_iterator = decltype(m_points)::const_iterator;
  const_iterator begin() const { return m_points.begin(); }
  const_iterator end() const { return m_points.end(); }
};

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

TEST_F(PositionOnlyProviderTest, iterateContainer) {
  std::vector<Kernel::Point_3> points = {Kernel::Point_3(0, 0, 0),
                                         Kernel::Point_3(0, 1, 0)};
  auto bufferProvider = make_position_buffer_provider(points);
  float expectedValues[] = {0, 0, 0, 0, 1, 0};
  float* expectedValuePtr = expectedValues;
  auto iter = bufferProvider.begin();
  for (size_t count = 0;
       count < (sizeof(expectedValues) / sizeof(expectedValues[0]));
       ++count, ++iter) {
    EXPECT_EQ(*iter, *expectedValuePtr++);
  }
  EXPECT_EQ(iter, bufferProvider.end());
}

TEST_F(PositionOnlyProviderTest, iterateContainerNoCopy) {
  NoCopyContainer noCopyContainer;
  auto bufferProvider = make_position_buffer_provider(noCopyContainer);
  float expectedValues[] = {0, 0, 0, 0, 1, 0};
  float* expectedValuePtr = expectedValues;
  auto iter = bufferProvider.begin();
  for (size_t count = 0;
       count < (sizeof(expectedValues) / sizeof(expectedValues[0]));
       ++count, ++iter) {
    EXPECT_EQ(*iter, *expectedValuePtr++);
  }
  EXPECT_EQ(iter, bufferProvider.end());
}

TEST_F(PositionOnlyProviderTest, iteratePolyloop3) {
  auto bufferProvider = make_position_buffer_provider(loop3D);

  float* expectedValuePtr = unrolledValues;
  size_t count = 0;
  for (auto iter = bufferProvider.begin(); iter != bufferProvider.end();
       ++iter) {
    ASSERT_LT(count, sizeof(unrolledValues) / sizeof(unrolledValues[0]));
    EXPECT_EQ(*iter, *expectedValuePtr++);
    ++count;
  }
  EXPECT_EQ(count, sizeof(unrolledValues) / sizeof(unrolledValues[0]));
}

TEST_F(PositionOnlyProviderTest, iteratePolyloop2) {
  auto bufferProvider = make_position_buffer_provider(loop2D);

  float* expectedValuePtr = unrolledValues;
  size_t count = 0;
  for (auto iter = bufferProvider.begin(); iter != bufferProvider.end();
       ++iter) {
    ASSERT_LT(count, sizeof(unrolledValues) / sizeof(unrolledValues[0]));
    EXPECT_EQ(*iter, *expectedValuePtr++);
    ++count;
  }
  EXPECT_EQ(count, sizeof(unrolledValues) / sizeof(unrolledValues[0]));
}
