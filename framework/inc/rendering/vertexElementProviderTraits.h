#ifndef _FRAMEWORK_RENDERING_VERTEX_ELEMENT_PROVIDER_TRAITS_H_
#define _FRAMEWORK_RENDERING_VERTEX_ELEMENT_PROVIDER_TRAITS_H_

// Default storage strategy for element providers within the
// ElementBufferProvider is pointer storage.
template <typename ElementProvider>
class ElementProviderStorageStrategy {
 protected:
  ElementProviderStorageStrategy(const ElementProvider& provider)
      : m_provider(&provider) {}
  // We don't allow creation from temporaries, as we expect the provider to
  // remain valid over the life of the VertexElementBufferProvider.
  ElementProviderStorageStrategy(const ElementProvider&& provider) = delete;

  const ElementProvider* m_provider;
};

// A lightweight provider can inherit from here to allow for stack based
// storage of the provider. The provider itself must be copy-constructible.
template <typename ElementProvider>
class CopyProviderStorageStrategy {
 protected:
  CopyProviderStorageStrategy(const ElementProvider& provider)
      : m_lightWeightProvider(provider), m_provider(&m_lightWeightProvider) {}

  CopyProviderStorageStrategy(ElementProvider&& provider)
      : m_lightWeightProvider(provider), m_provider(&m_lightWeightProvider) {}

  const ElementProvider m_lightWeightProvider;
  const ElementProvider* m_provider;
};

// A VertexElement provider for the rendering system is a class that is capable
// of furnishing an iterator over vertices, the provider traits classes specify
// the element provider's, and also provide a description of the type of
// primitives that it is providing. This information is used by the rendering
// system's classes to populate appropriate vertex buffers and transmit them to
// the GPU.
template <typename ElementProvider, typename VertexElement>
struct VertexElementProviderTraits {
  using provider_type = ElementProvider;
  using const_iterator = typename ElementProvider::const_iterator;
  using storage_strategy = ElementProviderStorageStrategy<ElementProvider>;
};

#endif  // _FRAMEWORK_RENDERING_VERTEX_ELEMENT_PROVIDER_TRAITS_H_
