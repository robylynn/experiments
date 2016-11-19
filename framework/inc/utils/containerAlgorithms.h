#ifndef _CONTAINER_ALGORITHMS_H_
#define _CONTAINER_ALGORITHMS_H_

#include <glog/logging.h>

#include <algorithm>
#include <iterator>

#include <boost/iterator/iterator_facade.hpp>

#include "stencilIterator.h"
#include "blockIterator.h"
#include "tupleAlgorithms.h"

namespace utils {

/**
 * Count all elements between begin and end iterators (inclusive)
 */
template <typename InputIt>
typename std::iterator_traits<InputIt>::difference_type count_all(
    InputIt first, InputIt last) {
  return std::count_if(first, last,
                       [](const typename InputIt::reference) { return true; });
}

/**
 * Clamp values between low and high, using a comparator for determing relation
 * with low and hig  - the default comparator is std::less.
 */
template <class T, class Compare>
constexpr const T& clamp(const T& v, const T& lo, const T& hi, Compare comp) {
  return assert(!comp(hi, lo)), comp(v, lo) ? lo : comp(hi, v) ? hi : v;
}
template <typename T>
constexpr const T& clamp(const T& v, const T& lo, const T& hi) {
  return clamp(v, lo, hi, std::less<>());
}

}  // end namespace utils

#endif  //_CONTAINER_ALGORITHMS_H_
