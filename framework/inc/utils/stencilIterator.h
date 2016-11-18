#ifndef _FRAMEWORK_UTILS_STENCIL_ITERATOR_H_
#define _FRAMEWORK_UTILS_STENCIL_ITERATOR_H_

#include <glog/logging.h>

#include <memory>

namespace utils {
// Provides iteration over a local stencil defined as an array of index
// offsets. Each element of the stencil is iterated over, and, then, the
// underlying iterator is incremented. The user must ensure that a move to
// before begin or to after end is not encountered during a stencil iteration.
// Two coplete stencil iterators are equal if they have the same stencil index,
// and the same single iterator. To facilitate checking this, the stencil
// iterator also takes in the underlying container's begin and end iterators in
// its begin call.
template <typename SingleIterator, size_t N>
class stencil_iterator
    : public boost::iterator_facade<stencil_iterator<SingleIterator, N>,
                                    const typename SingleIterator::value_type,
                                    boost::forward_traversal_tag,
                                    const typename SingleIterator::value_type> {
 public:
  stencil_iterator(const SingleIterator& singleIter,
                   const std::array<int, N>& stencil)
      : m_stencil(&stencil),
        m_stencilIter(m_stencil->begin()),
        m_singleIter(singleIter) {}

  stencil_iterator(const SingleIterator& start, const SingleIterator& begin,
                   const SingleIterator& end, const std::array<int, N>& stencil)
      : m_stencil(&stencil),
        m_stencilIter(m_stencil->begin()),
        m_singleIter(start),
        m_singleBegin(begin),
        m_singleEnd(end) {}

 private:
  friend class boost::iterator_core_access;

  void increment() {
    if (++m_stencilIter == m_stencil->end()) {
      m_stencilIter = m_stencil->begin();
      ++m_singleIter;
    }
  }

  bool equal(const stencil_iterator& other) const {
    return (m_stencilIter == other.m_stencilIter) &&
           (m_singleIter == other.m_singleIter);
  }

  const typename SingleIterator::reference dereference() const {
    SingleIterator singleIterator = m_singleIter;

    // If the stencil value is positive
    for (int i = 0; i < *m_stencilIter; ++i) {
      LOG_IF(ERROR, singleIterator == m_singleEnd)
          << "Stencil iterator accessing beyond container end";
      ++singleIterator;
    }

    // If the stencil value is negative
    for (int i = 0; i > *m_stencilIter; --i) {
      LOG_IF(ERROR, singleIterator == m_singleBegin)
          << "Stencil iterator accessing before container begin";
      --singleIterator;
    }
    return *singleIterator;
  }

  const std::array<int, N>* m_stencil;
  typename std::array<int, N>::const_iterator m_stencilIter;
  SingleIterator m_singleIter;
  SingleIterator m_singleBegin;
  SingleIterator m_singleEnd;

 public:
  static stencil_iterator begin(const SingleIterator& start,
                                const SingleIterator& begin,
                                const SingleIterator& end,
                                const std::array<int, N>& stencil) {
    return stencil_iterator(start, begin, end, stencil);
  }
  static stencil_iterator end(const SingleIterator& end,
                              const std::array<int, N>& stencil) {
    return stencil_iterator(end, stencil);
  }
};

// Convenience ADL end iterator creator for stencil_circulator_iterator
template <typename SingleIterator, size_t N>
stencil_iterator<SingleIterator, N> make_stencil_iterator(
    const SingleIterator& iter, const std::array<int, N>& stencil) {
  return stencil_iterator<SingleIterator, N>(iter, stencil);
}

// Provides iteration over a local stencil defined as an array of index offsets
// from an underlying iterator that is based on a circulator. Each element of
// the stencil is iterated over by accessing the underlying iterator's
// circulator, and, then, the underlying iterator is incremented.
template <typename SingleIterator, size_t N>
class stencil_circulator_iterator
    : public boost::iterator_facade<
          stencil_circulator_iterator<SingleIterator, N>,
          const typename SingleIterator::value_type,
          boost::forward_traversal_tag,
          const typename SingleIterator::value_type> {
 public:
  stencil_circulator_iterator(const SingleIterator& singleIter,
                              const std::array<int, N>& stencil)
      : m_stencil(&stencil),
        m_stencilIter(m_stencil->begin()),
        m_singleIter(singleIter) {}

 private:
  friend class boost::iterator_core_access;

  void increment() {
    if (++m_stencilIter == m_stencil->end()) {
      m_stencilIter = m_stencil->begin();
      ++m_singleIter;
    }
  }

  bool equal(const stencil_circulator_iterator& other) const {
    typename SingleIterator::Circulator circulator =
        m_singleIter.current_circulator();
    typename SingleIterator::Circulator otherC =
        other.m_singleIter.current_circulator();
    return (m_stencilIter == other.m_stencilIter) &&
           (m_singleIter.current_circulator() ==
            other.m_singleIter.current_circulator()) &&
           (m_singleIter == other.m_singleIter);
  }

  const typename SingleIterator::reference dereference() const {
    typename SingleIterator::Circulator circulator =
        m_singleIter.current_circulator();

    // If the stencil value is positive
    for (int i = 0; i < *m_stencilIter; ++i) {
      ++circulator;
    }

    // If the stencil value is negative
    for (int i = 0; i > *m_stencilIter; --i) {
      --circulator;
    }
    return *circulator;
  }

  const std::array<int, N>* m_stencil;
  typename std::array<int, N>::const_iterator m_stencilIter;
  SingleIterator m_singleIter;

 public:
  static stencil_circulator_iterator begin(const SingleIterator& begin,
                                           const std::array<int, N>& stencil) {
    return stencil_circulator_iterator(begin, stencil);
  }
  static stencil_circulator_iterator end(const SingleIterator& end,
                                         const std::array<int, N>& stencil) {
    return stencil_circulator_iterator(end, stencil);
  }
};

// Convenience ADL end iterator creator for stencil_circulator_iterator
template <typename SingleIterator, size_t N>
stencil_circulator_iterator<SingleIterator, N> make_stencil_circulator_iterator(
    const SingleIterator& iter, const std::array<int, N>& stencil) {
  return stencil_circulator_iterator<SingleIterator, N>(iter, stencil);
}

}  // end namespace utils

#endif  //_FRAMEWORK_UTILS_STENCIL_ITERATOR_H_
