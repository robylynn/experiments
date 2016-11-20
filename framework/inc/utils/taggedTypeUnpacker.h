#ifndef _FRAMEWORK_UTILS_TAGGED_TYPE_UNPACKER_H_
#define _FRAMEWORK_UTILS_TAGGED_TYPE_UNPACKER_H_

#include <tuple>

namespace utils {

/**
 * A tagged type is a type that provides a ::type typedef. This type can be a
 * simple type, or a std::tuple where each element of the tuple is a tagged
 * type. The utility code here converts tagged types to underlying types
 * (and underlying type packs in case of a tuple).
 */
template <typename... TaggedTypes>
struct TaggedTypeUnpacker {
  using type = std::tuple<typename TaggedTypes::type...>;
};

// The base template works for simple tagged types.
template <typename TaggedTypeHolder>
struct TagToType {
  using type = typename TaggedTypeHolder::type;
};

// Specialize for std::tuple
template <typename... Tags>
struct TagToType<std::tuple<Tags...>> {
  using type = typename TaggedTypeUnpacker<Tags...>::type;
};

template <typename TaggedType>
using tag_to_type_t = typename TagToType<TaggedType>::type;

}  // end of namespace utils

#endif  //_FRAMEWORK_UTILS_TAGGED_TYPE_UNPACKER_H_
