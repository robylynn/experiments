#include <gtest/gtest.h>

#include <CGAL/circulator.h>

#include "containerAlgorithms.h"

using namespace utils;

TEST(StencilCirculatorIteratorTest, iterateTest) {
  std::vector<int> values = {0, 1, 2, 3};

  using Circulator =
      CGAL::Circulator_from_iterator<typename std::vector<int>::const_iterator>;
  using CirculatorContainer = CGAL::Container_from_circulator<Circulator>;
  using SingleIterator = decltype(std::declval<CirculatorContainer&>().begin());
  Circulator circulator(values.begin(), values.end());
  CirculatorContainer circularContainer(circulator);

  std::array<int, 2> stencil = {0, 1};
  int stencilValues[] = {0, 1, 1, 2, 2, 3, 3, 0};
  size_t index = 0;
  for (auto iter = utils::make_stencil_circulator_iterator(
           circularContainer.begin(), stencil);
       iter != utils::make_stencil_circulator_iterator(circularContainer.end(),
                                                       stencil);
       ++iter) {
    ASSERT_LT(index, stencil.size() * values.size());
    EXPECT_EQ(*iter, stencilValues[index++]);
  }
  EXPECT_EQ(index, stencil.size() * values.size());
}
