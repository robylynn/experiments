#ifndef _FRAMEWORK_GEOMETRY_ATTRIBUTES_TO_TYPES_H_
#define _FRAMEWORK_GEOMETRY_ATTRIBUTES_TO_TYPES_H_

/**
 * Utility code to convert attribute packs to underlying type packs.
 */

template <typename... Attributes>
struct AttributeUnpacker {
  using type = std::tuple<typename Attributes::type...>;
};

template <typename AttributesHolder>
struct AttributesToTypes {};

template <typename... Attributes>
struct AttributesToTypes<std::tuple<Attributes...>> {
  using type = typename AttributeUnpacker<Attributes...>::type;
};

template <typename AttribsTuple>
using attributes_to_types_t = typename AttributesToTypes<AttribsTuple>::type;

#endif  //_FRAMEWORK_GEOMETRY_ATTRIBUTES_TO_TYPES_H_
