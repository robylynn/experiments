#ifndef _CONTAINER_ALGORITHMS_H_
#define _CONTAINER_ALGORITHMS_H_

#include <algorithm>

namespace utils {

template <typename InputIt>
typename std::iterator_traits<InputIt>::difference_type count_all(
    InputIt first, InputIt last) {
  return std::count_if(first, last,
                       [](const typename InputIt::reference) { return true; });
}

}  // end namespace utils

#endif  //_CONTAINER_ALGORITHMS_H_
