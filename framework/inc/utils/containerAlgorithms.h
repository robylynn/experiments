#ifndef _CONTAINER_ALGORITHMS_H_
#define _CONTAINER_ALGORITHMS_H_

#include <glog/logging.h>

#include <algorithm>
#include <iterator>

#include <boost/iterator/iterator_facade.hpp>

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
          typename DerefType = std::array<
              typename std::iterator_traits<SingleIter>::value_type, N>>
class tuple_iterator
    : public boost::iterator_facade<tuple_iterator<SingleIter, N, DerefType>,
                                    DerefType, boost::forward_traversal_tag,
                                    DerefType> {
 private:
  using SingleIterValueType =
      typename std::iterator_traits<SingleIter>::value_type;

  friend class boost::iterator_core_access;
  tuple_iterator(const SingleIter& begin, const SingleIter& end)
      : m_singleCurrent(begin), m_singleEnd(end) {
    populateNextTupleBuffer();
  }

  tuple_iterator(const SingleIter& end)
      : m_singleCurrent(end), m_singleEnd(end) {}

  void populateNextTupleBuffer() const {
    SingleIter iter = m_singleCurrent;
    // Read ahead and buffer if we are currently not at the end of the single
    // iterator
    if (iter != m_singleEnd) {
      for (int i = 0; i < N; ++i) {
        LOG_IF(ERROR, iter == m_singleEnd) << "tuple_iterator tuple-size "
                                              "should be an exact divisor of "
                                              "single iterator range";
        m_readAhead[i] = *iter;
        ++iter;
      }
    }
  }

  void increment() {
    for (int i = 0; i < N; ++i) {
      LOG_IF(ERROR, m_singleCurrent++ == m_singleEnd)
          << "tuple_iterator indexes beyond single iterator range";
    }
    populateNextTupleBuffer();
  }

  // TODO msati3: Critical: make this variadic
  DerefType dereference() const {
    return {m_readAhead[0], m_readAhead[1], m_readAhead[2]};
  }

  bool equal(const tuple_iterator& other) const {
    return (other.m_singleCurrent == m_singleCurrent);
  }

 public:
  static tuple_iterator begin(SingleIter begin, SingleIter end) {
    return tuple_iterator(begin, end);
  }
  static tuple_iterator end(SingleIter end) { return tuple_iterator(end); }

  // We also provide an end iterator that takes a query tuple iterator (that
  // has both begin and end states of the internal "single" iterator), and use
  // it to create the corresponding end tuple_iterator.
  static tuple_iterator end(const tuple_iterator& queryIter) {
    return end(queryIter.m_singleEnd);
  }

 private:
  mutable std::array<SingleIterValueType, N> m_readAhead;
  SingleIter m_singleCurrent;
  SingleIter m_singleEnd;
};

// Convenience ADL end iterator creator for tuple_iterator
template <size_t N, typename DerefType, typename SingleIter>
tuple_iterator<SingleIter, N, DerefType> make_tuple_iterator(
    const SingleIter& iterBegin, const SingleIter& iterEnd) {
  return tuple_iterator<SingleIter, N, DerefType>::begin(iterBegin, iterEnd);
}

template <size_t N, typename DerefType, typename SingleIter>
tuple_iterator<SingleIter, N, DerefType> make_tuple_iterator(
    const SingleIter& iterEnd) {
  return tuple_iterator<SingleIter, N, DerefType>::end(iterEnd);
}

template <size_t N, typename DerefType, typename SingleIter>
tuple_iterator<SingleIter, N, DerefType> make_end_tuple_iterator(
    const tuple_iterator<SingleIter, N, DerefType>& queryIter) {
  return tuple_iterator<SingleIter, N, DerefType>::end(queryIter);
}

}  // end namespace utils

#endif  //_CONTAINER_ALGORITHMS_H_
