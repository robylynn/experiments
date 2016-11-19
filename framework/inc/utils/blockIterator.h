#ifndef _FRAMEWORK_UTILS_BLOCK_ITERATOR_H_
#define _FRAMEWORK_UTILS_BLOCK_ITERATOR_H_

namespace utils {

/** Create an iterator over an n-block by taking n entries together from
 * an iterator that provides one entry at a time.
 * \req: The DerefType must have a constructor that accepts N elements of
 * type SingleIter::value_type
 */
template <typename SingleIter, size_t N,
          typename DerefType = std::array<
              typename std::iterator_traits<SingleIter>::value_type, N>>
class block_iterator
    : public boost::iterator_facade<block_iterator<SingleIter, N, DerefType>,
                                    DerefType, boost::forward_traversal_tag,
                                    DerefType> {
 private:
  using SingleIterValueType =
      typename std::iterator_traits<SingleIter>::value_type;

  friend class boost::iterator_core_access;
  block_iterator(const SingleIter& begin, const SingleIter& end)
      : m_singleCurrent(begin), m_singleEnd(end) {
    populateNextTupleBuffer();
  }

  block_iterator(const SingleIter& end)
      : m_singleCurrent(end), m_singleEnd(end) {}

  void populateNextTupleBuffer() const {
    SingleIter iter = m_singleCurrent;
    // Read ahead and buffer if we are currently not at the end of the single
    // iterator
    if (iter != m_singleEnd) {
      for (int i = 0; i < N; ++i) {
        LOG_IF(ERROR, iter == m_singleEnd) << "block_iterator block-size "
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
          << "block_iterator indexes beyond single iterator range";
    }
    populateNextTupleBuffer();
  }

  // TODO msati3: Critical - make variadic
  DerefType dereference() const {
    std::make_integer_sequence<size_t, N> indexSequence;
    return {m_readAhead[0], m_readAhead[1], m_readAhead[2]};
  }

  bool equal(const block_iterator& other) const {
    return (other.m_singleCurrent == m_singleCurrent);
  }

 public:
  static block_iterator begin(const SingleIter& begin, const SingleIter& end) {
    return block_iterator(begin, end);
  }
  static block_iterator end(const SingleIter& end) {
    return block_iterator(end);
  }

  // We also provide an end iterator that takes a query block iterator (that
  // has both begin and end states of the internal "single" iterator), and use
  // it to create the corresponding end block_iterator.
  static block_iterator end(const block_iterator& queryIter) {
    return end(queryIter.m_singleEnd);
  }

 private:
  mutable std::array<SingleIterValueType, N> m_readAhead;
  SingleIter m_singleCurrent;
  SingleIter m_singleEnd;
};

// Convenience ADL end iterator creator for block_iterator
template <size_t N, typename DerefType, typename SingleIter>
block_iterator<SingleIter, N, DerefType> make_block_iterator(
    const SingleIter& iterBegin, const SingleIter& iterEnd) {
  return block_iterator<SingleIter, N, DerefType>::begin(iterBegin, iterEnd);
}

template <size_t N, typename DerefType, typename SingleIter>
block_iterator<SingleIter, N, DerefType> make_block_iterator(
    const SingleIter& iterEnd) {
  return block_iterator<SingleIter, N, DerefType>::end(iterEnd);
}

template <size_t N, typename DerefType, typename SingleIter>
block_iterator<SingleIter, N, DerefType> make_end_block_iterator(
    const block_iterator<SingleIter, N, DerefType>& queryIter) {
  return block_iterator<SingleIter, N, DerefType>::end(queryIter);
}

} // end namespace utils

#endif  //_FRAMEWORK_UTILS_BLOCK_ITERATOR_H_
