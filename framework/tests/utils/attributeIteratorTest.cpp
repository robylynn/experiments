#include <gtest/gtest.h>

#include "tupleElementIteratorAdaptor.h"

TEST(AttributeIteratorTest, intrinsicTypeTest) {
  std::vector<std::tuple<int>> container{std::tuple<int>{1},
                                         std::tuple<int>{2}};
  std::vector<int> expected = {1, 2};
  auto iterExpected = expected.begin();

  for (auto iter = utils::make_tuple_element_iterator<int>(container.begin());
       iter != utils::make_tuple_element_iterator<int>(container.end());
       ++iter) {
    EXPECT_EQ(*iter, *iterExpected++);
  }
  EXPECT_EQ(iterExpected, expected.end());
}
