#ifndef _FRAMEWORK_RENDERING_POSITION_ONLY_BUFFER_PROVIDER_H_
#define _FRAMEWORK_RENDERING_POSITION_ONLY_BUFFER_PROVIDER_H_

#include <functional>

#include <boost/iterator/iterator_facade.hpp>

#include "renderingConstants.h"
#include "vertexElementBufferProvider.h"
#include "vertexElement.h"
#include "vertexData.h"

// Adapts a class that provides begin and end over a single VertexElement type
// only to be a suitable vertex element buffer provider by providing begin and
// end wrappers that accept and VertexElement, and forwarding the calls to the
// VertexElement provider. This allows for simple STL containers to be used as
// VertexElement providers.
template <typename ElementType, typename ElementProvider>
class SingleElementBufferProviderAdaptor {
 public:
  SingleElementBufferProviderAdaptor(const ElementProvider& provider)
      : m_provider(provider) {}
  SingleElementBufferProviderAdaptor(const ElementProvider&& provider) = delete;

  static auto begin(const ElementProvider* provider, const ElementType &
                    /*element*/) -> decltype(provider->begin()) {
    return provider->begin();
  }

  static auto end(const GeometryProvider* provider, const ElementType &
                  /*element*/) -> decltype(provider->end()) {
    return provider->end();
  }

  using const_iterator = decltype(SingleElementBufferProviderAdaptor::begin);
};

// This classadapts the VertexElementBufferProvider to provide vertex
// buffer iterators over each of the VertexElements in the set. Allows for a
// single DataProvider to be used to provide VertexElement iterators for a set
// of VertexElements.
template <typename ElementsProvider, typename... VertexElements>
class TupleBufferDataProvider {
 public:
  TupleBufferDataProvider(const ElementsProvider& provider,
                          VertexElements... elements)
      : m_provider(provider), m_elements(elements) {}

 private:
  std::tuple<VertexElements...> m_elements;
  VertexElementBufferProvider<
  const ElementsProvider* m_provider;
}

// Convenience typedefs for VertexElementBuffer providers
template <typename GeometryProvider>
using PositionOnlyBufferProvider =
    VertexElementBufferProvider<PositionVertexElement,
                                SingleElementBufferProviderAdaptor<
                                    PositionVertexElement, GeometryProvider>>;

template <typename ColorProvider>
using ColorOnlyBufferProvider = VertexElementBufferProvider<
    ColorVertexElement,
    SingleElementBufferProviderAdaptor<ColorVertexElement, ColorProvider>>;

// Takes a collection of VertexElementBuffer providers, and provides for
// visitation over each of them.
template <typename... ElementBufferProviders>
class ChainedBufferDataProvider {
 public:
  using ProvidedElement = std::tuple<ElementBufferProviders...>;

  void begin(

 private:
  std::tuple<Providers...> m_vertexElementBufferProviders;
};

#endif  //_FRAMEWORK_RENDERING_POSITION_ONLY_BUFFER_PROVIDER_H_
