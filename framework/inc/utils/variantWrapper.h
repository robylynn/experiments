#ifndef _VARIANT_WRAPPER_H_
#define _VARIANT_WRAPPER_H_

#include <functional>

#include <boost/variant.hpp>

// Allows for the creation of types corresponding to passed in
// types wrapped by std::reference wrapper to store references.
template <typename WrappedType>
struct ReferenceTypeWrapper {
  using type = std::reference_wrapper<WrappedType>;
};

// Allows for the creation of types corresponding to passed in
// types wrapped by std::reference wrapper, to store const references.
template <typename WrappedType>
struct ConstReferenceTypeWrapper {
  using type = std::reference_wrapper<const WrappedType>;
};

// Given a variant that wraps another, invokes operations on the wrapped
// variant type. The operation must inherit from boost::static_visitor for the
// wrapped type.
template <typename WrappingType, typename Operation>
class WrappedVariantInvoker
    : public boost::static_visitor<
          typename WrappingType::type::type::result_type> {
 public:
  WrappedVariantInvoker(const Operation& operation) : m_operation(operation) {}

  template <typename WrappingVariantType>
  typename WrappingType::type::type::result_type operator()(
      const WrappingVariantType& wrappingType) {
    return m_operation(wrappingType.get());
  }

 private:
  const Operation m_operation;
};

#endif  //_VARIANT_WRAPPER_H_
