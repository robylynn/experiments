#include <gtest/gtest.h>

#include "containerAlgorithms.h"

using namespace utils;

TEST(TupleIteratorTest, iterateTest) {
  std::vector<int> values = {0, 1, 2, 3, 4, 5};
  constexpr size_t TUPLE_SIZE = 3;
  std::array<int, TUPLE_SIZE> tupleIteration[2] = {{0, 1, 2}, {3, 4, 5}};
  size_t index = 0;
  for (auto iter = make_tuple_iterator<TUPLE_SIZE, std::array<int, TUPLE_SIZE>>(
           values.begin(), values.end());
       iter != make_tuple_iterator<TUPLE_SIZE, std::array<int, TUPLE_SIZE>>(
                   values.end());
       ++iter) {
    ASSERT_LT(index, 2);
    EXPECT_EQ(*iter, tupleIteration[index++]);
  }
  EXPECT_EQ(index, 2);
}
