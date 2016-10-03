#ifndef _PARTIAL_ORDER_H_
#define _PARTIAL_ORDER_H_

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

#endif  //_PARTIAL_ORDER_H_
