#ifndef _FRAMEWORK_RENDERING_VERTEX_ELEMENT_PROVIDER_TRAITS_H_
#define _FRAMEWORK_RENDERING_VERTEX_ELEMENT_PROVIDER_TRAITS_H_

// A VertexElement provider for the rendering system is a class that is capable
// of furnishing an iterator over vertices, the provider traits classes specify
// the element provider's, and also provide a description of the type of
// primitives that it is providing. This information is used by the rendering
// system's classes to populate appropriate vertex buffers and transmit them to
// the GPU.
template <typename ElementProvider, typename VertexElement>
struct VertexElementProviderTraits {
  using provider_type = ElementProvider;
  using provided_type = ElementProvider;
  using const_iterator = typename ElementProvider::const_iterator;
  using storage_strategy = ElementProviderStorageStrategy<ElementProvider>;
};

#endif  // _FRAMEWORK_RENDERING_VERTEX_ELEMENT_PROVIDER_TRAITS_H_
