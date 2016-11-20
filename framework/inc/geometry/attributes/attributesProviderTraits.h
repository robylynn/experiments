#ifndef _FRAMEWORK_GEOMETRY_ATTRIBUTES_PROVIDER_TRAITS_H_
#define _FRAMEWORK_GEOMETRY_ATTRIBUTES_PROVIDER_TRAITS_H_

#include <storageStrategies.h>
#include <simplexTypes.h>

/**
 * The geometry framework consists of geometry representations that are modeled
 * as attribute providers -- they are capable of providing attributes per
 * iterable component of their representation (this may, for example be a
 * simplex, in case the geometry is modeled as such). These are certain
 * concepts each such provider must conform to, and, the types for these
 * concepts may be specialized through the traits below. Each provider is
 * allowed to customize either the traits class fully, or, influence some
 * aspect of the traits class by specializing some of parameterized types that
 * are used by the base traits.
 */

/**
 * Governs the storage of an attribute provider. By default, an attribute
 * provider will be stored as a pointer (as it is a heavy object). Proxy
 * providers (adaptors) can ask to be stored by copy.
 */
template <typename AttributesProvider>
class AttributesProviderStorageStrategy
    : public utils::PointerStorageStrategy<AttributesProvider> {
 protected:
  using utils::PointerStorageStrategy<
      AttributesProvider>::PointerStorageStrategy;
};

/**
 * Traits a const attribute provider must expose. It should allow for
 * iteration over its attribute set, while also state which simplex type it is
 * providing.
 */
template <typename ResourceProvider>
struct ConstAttributesProviderTraits {
  using provider_type = ResourceProvider;
  using const_iterator = typename ResourceProvider::const_iterator;
  using storage_strategy = AttributesProviderStorageStrategy<ResourceProvider>;
};

/**
 * A non-const attribute provider must also expose an iterator type.
 */
template <typename ResourceProvider>
struct NonConstAttributesProviderTraits
    : public ConstAttributesProviderTraits<ResourceProvider> {
  using interator = typename ResourceProvider::iterator;
};

/**
 * A simplicial decomposition provider must also annotate with the type of
 * simplex it is provided.
 */
template <typename ResourceProvider, typename SimplexType>
struct SimplicialDecompositionProviderTraits
    : public ConstAttributesProviderTraits<ResourceProvider> {
  using simplex_type = SimplexType;
};

#endif  //_FRAMEWORK_GEOMETRY_ATTRIBUTES_PROVIDER_TRAITS_H_
