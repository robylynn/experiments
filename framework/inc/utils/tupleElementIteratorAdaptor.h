#ifndef _FRAMEWORK_UTILS_TUPLE_ELEMENT_ITERATOR_ADAPTOR_H_
#define _FRAMEWORK_UTILS_TUPLE_ELEMENT_ITERATOR_ADAPTOR_H_

#include <functional>

#include <boost/iterator/transform_iterator.hpp>

namespace utils {
// An attribute set iterator is an iterator that provides a set of
// heterogeneous types (std::tuple with unique tuple elements) on
// dereferencing. This iterator adaptor adapts such adaptors to provider
// iteration over individual attributes.
template <typename BaseIter>
class TupleElementIteratorAdaptor {
  template <typename Attrib>
  using AttribAccessFunctor =
      std::function<Attrib&(decltype(*std::declval<BaseIter>()))>;
  template <typename Attrib>
  using Iter = boost::transform_iterator<AttribAccessFunctor<Attrib>, BaseIter>;

 public:
  template <typename Attrib>
  static Iter<Attrib> attribute_iter(const BaseIter& baseIter) {
    AttribAccessFunctor<Attrib> functor =
        static_cast<Attrib& (*)(decltype(*baseIter)&)>(std::get<Attrib>);
    return Iter<Attrib>(baseIter, functor);
  }
};

template <typename Attrib, typename Tuple, typename Iterator>
using TupleElementIterator =
    boost::transform_iterator<std::function<Attrib&(Tuple&)>, Iterator>;

// Allow iteration over
template <typename Attrib, typename BaseIter>
auto make_tuple_element_iterator(const BaseIter& baseIter)
    -> boost::transform_iterator<std::function<Attrib&(decltype(*baseIter)&)>,
                                 BaseIter> {
  return TupleElementIteratorAdaptor<BaseIter>::template attribute_iter<Attrib>(
      baseIter);
}

}  // end of namespace utils

#endif  //_FRAMEWORK_UTILS_TUPLE_ELEMENT_ITERATOR_ADAPTOR_H_
