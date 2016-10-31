#ifndef _FRAMEWORK_RENDERING_SINGLE_ELEMENT_BUFFER_PROVIDER_H_
#define _FRAMEWORK_RENDERING_SINGLE_ELEMENT_BUFFER_PROVIDER_H_

#include <functional>

#include <boost/iterator/iterator_facade.hpp>

#include "renderingConstants.h"
#include "vertexElementBufferProvider.h"

// Adapts a class that provides begin and end over a single VertexElement type
// only to be a suitable vertex element buffer provider by providing begin and
// end wrappers that accept and VertexElement, and forwarding the calls to the
// VertexElement provider. This allows for simple STL containers to be used as
// VertexElement providers.
template <typename ElementProvider, typename ElementType>
class SingleElementBufferProviderAdaptor {
 private:
  const ElementProvider* m_provider;

 public:
  using const_iterator = decltype(m_provider->begin());

  SingleElementBufferProviderAdaptor(
      const SingleElementBufferProviderAdaptor& adaptor) {
    m_provider = adaptor.m_provider;
  }

  SingleElementBufferProviderAdaptor& operator=(
      const SingleElementBufferProviderAdaptor& adaptor) {
    m_provider = adaptor.m_provider;
  }

  SingleElementBufferProviderAdaptor(
      const SingleElementBufferProviderAdaptor&& adaptor) {
    m_provider = adaptor.m_provider;
  }

  SingleElementBufferProviderAdaptor(const ElementProvider& provider)
      : m_provider(&provider) {}

  const ElementProvider* get() const { return m_provider; }

  SingleElementBufferProviderAdaptor(const ElementProvider&& provider) = delete;

  const_iterator begin(const ElementType& element) const {
    return m_provider->begin();
  }

  const_iterator end(const ElementType& element) const {
    return m_provider->end();
  }
};

// A single element buffer provider adaptor specializes the storage policy to
// store by value. This allows for nicer client syntax through implicit
// SingleElementBufferProviderAdaptor(ElementProvider) temporary creation, and
// binding of this temporary to VertexElementBufferProvider.
template <typename ElementProvider, typename ElementType>
class ElementProviderStorageStrategy<
    SingleElementBufferProviderAdaptor<ElementProvider, ElementType>> {
 protected:
  ElementProviderStorageStrategy(const SingleElementBufferProviderAdaptor<
      ElementProvider, ElementType>& provider)
      : m_lightWeightProvider(provider), m_provider(&m_lightWeightProvider) {}

  const SingleElementBufferProviderAdaptor<ElementProvider, ElementType>
      m_lightWeightProvider;
  const SingleElementBufferProviderAdaptor<ElementProvider, ElementType>*
      m_provider;
};

// Convenience typedefs for common single element buffer providers
template <typename GeometryProvider>
using PositionOnlyBufferProvider = VertexElementBufferProvider<
    SingleElementBufferProviderAdaptor<GeometryProvider, PositionVertexElement>,
    PositionVertexElement>;

template <typename ColorProvider>
using ColorOnlyBufferProvider = VertexElementBufferProvider<
    SingleElementBufferProviderAdaptor<ColorProvider, ColorVertexElement>,
    ColorVertexElement>;

#endif  //_FRAMEWORK_RENDERING_SINGLE_ELEMENT_BUFFER_PROVIDER_H_
