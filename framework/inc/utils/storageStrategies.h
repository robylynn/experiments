#ifndef _FRAMEWORK_UTILS_RESOURCE_PROVIDER_STORAGE_STRATEGIES_H_
#define _FRAMEWORK_UTILS_RESOURCE_PROVIDER_STORAGE_STRATEGIES_H_

namespace utils {
// A resource provider is an object that provides a particular (set of)
// resources. Often, adaptors are written that transform an underlying resource
// provider to provider a transformed version of the resource.
//
// This file allows easy customization of a provider's storage strategy
// in a class that uses object composition for provider storage. Each resource
// provider advertizes its Storage strategy via a type-traits system. The
// classes that compose a resource provider can then use the types traits.

// Default storage strategy for element providers within the
// ElementBufferProvider is pointer storage.
template <typename ResourceProvider>
class ResourceProviderStorageStrategy {
 protected:
  ResourceProviderStorageStrategy(const ResourceProvider& provider)
      : m_provider(&provider) {}
  // We don't allow creation from temporaries, as we expect the object to
  // remain valid over the life of the Object.
  ResourceProviderStorageStrategy(const ResourceProvider&& provider) = delete;

  const ResourceProvider* m_provider;
};

// A lightweight provider can inherit from here to allow for stack based
// storage of the provider. The provider itself must be copy-constructible.
template <typename ResourceProvider>
class CopyProviderStorageStrategy {
 protected:
  CopyProviderStorageStrategy(const ResourceProvider& provider)
      : m_lightWeightProvider(provider), m_provider(&m_lightWeightProvider) {}

  CopyProviderStorageStrategy(ResourceProvider&& provider)
      : m_lightWeightProvider(provider), m_provider(&m_lightWeightProvider) {}

  const ResourceProvider m_lightWeightProvider;
  const ResourceProvider* m_provider;
};

}  // end of namespace utils

#endif  // FRAMEWORK_UTILS_RESOURCE_PROVIDER_STORAGE_STRATEGIES_H_
