#ifndef _FRAMEWORK_UTILS_TUPLE_ELEMENT_ITERATOR_ADAPTOR_H_
#define _FRAMEWORK_UTILS_TUPLE_ELEMENT_ITERATOR_ADAPTOR_H_

#include <functional>

#include <boost/iterator/iterator_adaptor.hpp>

namespace utils {
// An attribute set iterator is an iterator that provides a set of
// heterogeneous types (std::tuple with unique tuple elements) on
// dereferencing. This iterator adaptor adapts such adaptors to provider
// iteration over individual attributes.
template <typename BaseIter>
class TupleElementIteratorAdaptor {
 public:
  // Bind transform function on construction.
  template <typename Attrib>
  class ElementIter
      : public boost::iterator_adaptor<ElementIter<Attrib>, BaseIter, Attrib> {
   public:
    ElementIter(const BaseIter& baseIter) : m_baseIter(baseIter) {}

   private:
    friend class boost::iterator_core_access;
    void increment() { ++m_baseIter; }
    bool equal(const ElementIter& other) const {
      return m_baseIter == other.m_baseIter;
    }
    void advance(size_t step) { m_baseIter += step; }
    Attrib& dereference() const { return std::get<Attrib>(*m_baseIter); }
    BaseIter m_baseIter;
  };

  template <typename Attrib>
  static ElementIter<Attrib> attribute_iter(const BaseIter& baseIter) {
    return ElementIter<Attrib>(baseIter);
  }
};

// Allow iteration over
template <typename Attrib, typename BaseIter>
typename TupleElementIteratorAdaptor<BaseIter>::template ElementIter<Attrib>
make_tuple_element_iterator(const BaseIter& baseIter) {
  return TupleElementIteratorAdaptor<BaseIter>::template attribute_iter<Attrib>(
      baseIter);
}

}  // end of namespace utils

#endif  //_FRAMEWORK_UTILS_TUPLE_ELEMENT_ITERATOR_ADAPTOR_H_
