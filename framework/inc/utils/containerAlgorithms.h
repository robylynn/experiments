#ifndef _CONTAINER_ALGORITHMS_H_
#define _CONTAINER_ALGORITHMS_H_

#include <glog/logging.h>

#include <algorithm>

#include <boost/iterator/iterator_adaptor.hpp>

namespace utils {

template <typename InputIt>
typename std::iterator_traits<InputIt>::difference_type count_all(
    InputIt first, InputIt last) {
  return std::count_if(first, last,
                       [](const typename InputIt::reference) { return true; });
}

// Create an iterator that creates an n-tuple by taking n entries together from
// an iterator that provides one entry at a time. The DerefType must have a
// constructor that accepts N elements of type SingleIter::value_type
template <typename SingleIter, size_t N,
          typename DerefType = std::array<typename SingleIter::value_type, N>>
class tuple_iterator
    : public boost::iterator_adaptor<tuple_iterator<SingleIter, N, DerefType>,
                                     SingleIter, DerefType, boost::use_default,
                                     DerefType> {
 private:
  friend class boost::iterator_core_access;
  tuple_iterator(const SingleIter& begin, const SingleIter& end)
      : m_singleIter(begin),
        m_singleEnd(end),
        tuple_iterator::iterator_adaptor_(begin) {}

  void increment() {
    for (int i = 0; i < N; ++i) {
      DLOG_IF(ERROR, m_singleIter == m_singleEnd)
          << "tuple_iterator indexes beyond single iterator range";
      m_combined[i] = *m_singleIter++;
    }
  }

  // TODO msati3: Critical: make this variadic
  DerefType dereference() const {
    DerefType ref(m_combined[0], m_combined[1], m_combined[2]);
    return ref;
  }

 public:
  static tuple_iterator begin(SingleIter begin, SingleIter end) {
    return tuple_iterator(begin, end);
  }
  static tuple_iterator end(SingleIter end) { return tuple_iterator(end, end); }

  // We also provide an end iterator that takes a query tuple iterator (that
  // has both begin and end states of the internal "single" iterator), and use
  // it to create the corresponding end tuple_iterator.
  static tuple_iterator end(const tuple_iterator& queryIter) {
    return end(queryIter.m_singleEnd);
  }

 private:
  std::array<typename SingleIter::value_type, N> m_combined;
  SingleIter m_singleIter;
  SingleIter m_singleEnd;
};

// Convenience ADL end iterator creator for tuple_iterator
template <typename SingleIter, size_t N, typename DerefType>
tuple_iterator<SingleIter, N, DerefType> make_end_tuple_iterator(
    const tuple_iterator<SingleIter, N, DerefType>& queryIter) {
  return tuple_iterator<SingleIter, N, DerefType>::end(queryIter);
}

}  // end namespace utils

#endif  //_CONTAINER_ALGORITHMS_H_
