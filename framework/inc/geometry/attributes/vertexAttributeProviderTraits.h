#ifndef _FRAMEWORK_GEOMETRY_VERTEX_ATTRIBUTE_PROVIDER_TRAITS_H_
#define _FRAMEWORK_GEOMETRY_VERTEX_ATTRIBUTE_PROVIDER_TRAITS_H_

#include <resourceProviderStorageStrategies.h>

// A VertexAttribute provider a class that is capable of furnishing an iterator
// over vertices, the provider traits classes specify
// the element provider's. This information may be used, among other
// things, by the rendering system's classes to populate appropriate vertex
// buffers and transmit them to the GPU for a given geometry representation.

// The BaseVertexProvider traits provide a convenience for obtaining
// const_iterator and storage strategy traits of an adaptor
template <typename ResourceProvider>
struct SimpleVertexAttribProviderTraits {
  using simple_provider = ResourceProvider;
  using const_iterator = typename ResourceProvider::const_iterator;
  using storage_strategy = AttributeProviderStorageStrategy<ResourceProvider>;
}

template <typename ResourceProvider>
struct VertexAttributeProviderTraits
    : public SimpleVertexAttribProviderTraits<ResourceProvider> {
  using simplex_type = PointList;
  using provider_type = typename SimpleVertexAttribProviderTraits<
      ResourceProvider>::simple_provider;
  using provided_type = AttributeProvider;
};

#endif  // _FRAMEWORK_GEOMETRY_VERTEX_ATTRIBUTE_PROVIDER_TRAITS_H_
