#include <tuple>
#include <functional>

#include <boost/iterator/transform_iterator.hpp>

// A flat container of a hetrogeneous attribute set.
template <typename... Attribs>
class GenericAttributeSetContainer {
  using FirstAttrib = std::tuple_element_t<0, std::tuple<Attribs...>>;
  template <typename Attrib>
  using AttribAccessFunctor = std::function<Attrib&(std::tuple<Attribs...>&)>;
  template <typename Attrib>
  using Iter = boost::transform_iterator<
      AttribAccessFunctor<Attrib>,
      typename std::vector<std::tuple<Attribs...>>::iterator>;

 public:
  GenericAttributeSetContainer(
      std::vector<std::tuple<Attribs>...>&& attribsVector)
      : m_attribsVector(attribsVector) {}

  template <typename Attrib>
  Iter<Attrib> attrib_begin() {
    AttribAccessFunctor<Attrib> functor =
        static_cast<Attrib& (*)(std::tuple<Attribs...>&)>(
            std::get<Attrib, Attribs...>);
    return Iter<Attrib>(m_attribsVector.begin(), functor);
  }

  template <typename Attrib>
  Iter<Attrib> attrib_end() {
    AttribAccessFunctor<Attrib> functor =
        static_cast<Attrib& (*)(std::tuple<Attribs...>&)>(
            std::get<Attrib, Attribs...>);
    return Iter<Attrib>(m_attribsVector.end(), functor);
  }

  Iter<FirstAttrib> first_attrib_begin() {
    AttribAccessFunctor<FirstAttrib> functor =
        static_cast<FirstAttrib& (*)(std::tuple<Attribs...>&)>(
            std::get<FirstAttrib, Attribs...>);
    return Iter<FirstAttrib>(m_attribsVector.begin(), functor);
  }

  Iter<FirstAttrib> first_attrib_end() {
    AttribAccessFunctor<FirstAttrib> functor =
        static_cast<FirstAttrib& (*)(std::tuple<Attribs...>&)>(
            std::get<FirstAttrib, Attribs...>);
    return Iter<FirstAttrib>(m_attribsVector.end(), functor);
  }

 private:
  std::vector<std::tuple<Attribs...>> m_attribsVector;
};

template <typename... Attribs>
class AttributeSetContainer : public GenericAttributeSetContainer<Attribs...> {
};

// Single attrib holder also decorates class with begin and end iterators
template <typename Attribute>
class AttributeSetContainer<Attribute>
    : public GenericAttributeSetContainer<Attribute> {
  using Base = GenericAttributeSetContainer<Attribute>;
  using first_iter_ret_type =
      decltype(std::declval<Base>().first_attrib_begin());
  //using iter_ret_type =
  //    decltype(std::declval<Base>().attrib_begin<Attribute>());

 public:
  AttributeSetContainer(std::vector<std::tuple<Attribute>>&& attribList)
      : Base(std::move(attribList)) {}
  first_iter_ret_type begin() { return this->first_attrib_begin(); }
  first_iter_ret_type end() { return this->first_attrib_end(); }

  // iter_ret_type begin() { return this->attrib_begin<Attribute>(); }
  // iter_ret_type end() { return this->attrib_end<Attribute>(); }
};
