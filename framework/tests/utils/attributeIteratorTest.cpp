#include <gtest/gtest.h>

#include "tupleElementIteratorAdaptor.h"

class CompoundType {
 public:
  CompoundType(int a, int b) : m_a(a), m_b(b) {}
  bool operator==(const CompoundType& other) const {
    return ((m_a == other.m_a) && (m_b == other.m_b));
  }

 private:
  int m_a;
  int m_b;
};

TEST(AttributeIteratorTest, compoundTypeTest) {
  std::vector<std::tuple<CompoundType>> container;
  container.push_back(CompoundType(1, 1));
  container.push_back(CompoundType(2, 2));

  std::vector<CompoundType> expected = {CompoundType{1, 1}, CompoundType{2, 2}};
  auto iterExpected = expected.begin();

  for (auto iter =
           utils::make_tuple_element_iterator<CompoundType>(container.begin());
       iter !=
           utils::make_tuple_element_iterator<CompoundType>(container.end());
       ++iter) {
    EXPECT_EQ(*iter, *iterExpected++);
  }
  EXPECT_EQ(iterExpected, expected.end());
}
