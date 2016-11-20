#ifndef _FRAMEWORK_UTILS_RESOURCE_PROVIDER_STORAGE_STRATEGIES_H_
#define _FRAMEWORK_UTILS_RESOURCE_PROVIDER_STORAGE_STRATEGIES_H_

namespace utils {
/** A resource provider is an object that provides a particular (set of)
 * resources. Often, adaptors are written that transform an underlying resource
 * provider to provider a transformed version of the resource.
 *
 * This file allows easy customization of a provider's storage strategy
 * in a class that uses object composition for provider storage. Each resource
 * provider advertizes its storage strategy via a type-traits system. The
 * classes that compose a resource provider can then use the types traits.
 */

/** The composing class should store a pointer for this resource provider.
 * This is often the case when a concrete resource provider is being used
 * and stored in a containing class.
 */
template <typename ResourceProvider>
class PointerStorageStrategy {
 protected:
  PointerStorageStrategy(const ResourceProvider& provider)
      : m_provider(&provider) {}
  // We don't allow creation from temporaries, as we expect the object to
  // remain valid over the life of the Object.
  PointerStorageStrategy(const ResourceProvider&& provider) = delete;

  const ResourceProvider* m_provider;
};

/** A lightweight provider can inherit from here to allow for stack based
 * storage of the provider. The provider itself must be copy-constructible.
 */
template <typename ResourceProvider>
class CopyStorageStrategy {
 protected:
  CopyStorageStrategy(const ResourceProvider& provider)
      : m_lightWeightProvider(provider), m_provider(&m_lightWeightProvider) {}

  CopyStorageStrategy(ResourceProvider&& provider)
      : m_lightWeightProvider(provider), m_provider(&m_lightWeightProvider) {}

  const ResourceProvider m_lightWeightProvider;
  const ResourceProvider* m_provider;
};

}  // end of namespace utils

#endif  // FRAMEWORK_UTILS_RESOURCE_PROVIDER_STORAGE_STRATEGIES_H_
