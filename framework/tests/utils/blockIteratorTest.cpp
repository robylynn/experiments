#include <gtest/gtest.h>

#include <blockIterator.h>

using namespace utils;

TEST(BlockIteratorTest, iterateTest) {
  std::vector<int> values = {0, 1, 2, 3, 4, 5};
  constexpr size_t BLOCK_SIZE = 3;
  std::array<int, BLOCK_SIZE> blockIteration[2] = {{0, 1, 2}, {3, 4, 5}};
  size_t index = 0;
  for (auto iter = make_block_iterator<BLOCK_SIZE, std::array<int, BLOCK_SIZE>>(
           values.begin(), values.end());
       iter != make_block_iterator<BLOCK_SIZE, std::array<int, BLOCK_SIZE>>(
                   values.end());
       ++iter) {
    ASSERT_LT(index, 2);
    EXPECT_EQ(*iter, blockIteration[index++]);
  }
  EXPECT_EQ(index, 2);
}
