#ifndef _COMMON_TYPES_H_
#define _COMMON_TYPES_H_

#include <tuple>

// Partial ordering on tuples. Returns true if first < second.
// The partial ordering implies if first < second, then second > first, and
// thus, !PartialOrder can be used for > comparisons
template <typename Tuple, size_t N = std::tuple_size<Tuple>::value - 1>
class PartialOrder {
 public:
  bool operator()(const Tuple& first, const Tuple& second) {
    return (std::get<N>(first) < std::get<N>(second)) &&
           (PartialOrder<Tuple, N - 1>()(first, second));
  }
};

template <typename Tuple>
class PartialOrder<Tuple, 0> {
 public:
  bool operator()(const Tuple& /*first*/, const Tuple& /*second*/) {
    return true;
  }
};

typedef std::tuple<size_t, size_t, size_t> Index_3;
typedef std::tuple<int, int, int> Integer_3;

#endif  //_COMMON_TYPES_H_
