#ifndef _FRAMEWORK_RENDERING_VERTEXBUFFER_PROVIDER_TRAITS_H_
#define _FRAMEWORK_RENDERING_VERTEXBUFFER_PROVIDER_TRAITS_H_

template <typename BufferProvider>
struct VertexBufferProviderTraits {
  static const size_t vertexStart = BufferProvider::vertexStart;
  static const size_t maxBound = BufferProvider::maxBound;
  static const bool useIndexes = BufferProvider::useIndexes;
  using vertex_elements = typename BufferProvider::vertex_elements;
  using element_providers = typename BufferProvider::element_providers;
};

#endif  // _FRAMEWORK_RENDERING_VERTEXBUFFER_PROVIDER_TRAITS_H_
