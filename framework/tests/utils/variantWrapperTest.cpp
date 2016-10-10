#include <gtest/gtest.h>

#include <boost/variant.hpp>
#include <boost/variant/recursive_variant.hpp>
#include <boost/mpl/transform.hpp>

#include "variantWrapper.h"

// A straight-forward use of the variant wrapper.
class A {};
class B {};

using ABVariant = boost::variant<A, B>;
using RefABVariant = boost::make_variant_over<boost::mpl::transform<
    ABVariant::types, ConstReferenceTypeWrapper<boost::mpl::_1>>::type>::type;

// A use of variant wrapper conditioned on a passed in template type
template <typename VariantType>
class ConstVariantHolder {
  using ConstRefVariantType =
      typename boost::make_variant_over<typename boost::mpl::transform<
          typename VariantType::types,
          ConstReferenceTypeWrapper<boost::mpl::_1>>::type>::type;

 private:
  std::vector<ConstRefVariantType> m_holder;
};

TEST(VariantWrapperTest, wrapTest) {
  std::vector<RefABVariant> refABVector;
  A a;
  refABVector.push_back(std::cref(a));
  ConstVariantHolder<ABVariant> variantHolder;
}
