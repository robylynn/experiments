#ifndef _FRAMEWORK_RENDERING_TUPLE_ELEMENT_BUFFER_PROVIDER_H_
#define _FRAMEWORK_RENDERING_TUPLE_ELEMENT_BUFFER_PROVIDER_H_

#include "vertexElementBufferProvider.h"

// This classadapts the VertexElementBufferProvider to provide vertex
// buffer iterators over each of the VertexElements in the set. Allows for a
// single DataProvider to be used to provide VertexElement iterators for a set
// of VertexElements.
template <typename ElementsProvider, typename... ElementTypes>
class TupleBufferDataProvider {
 public:
  TupleBufferDataProvider(const ElementsProvider& provider)
      : m_provider(provider) {}
  // m_bufferProviders(m_provider, m_elements...) {}

  template <typename VertexElement>
  static auto begin(const ElementsProvider* provider,
                    const VertexElement& element)
      -> decltype(provider->begin(element)) {
    return provider->begin(element);
  }

  template <typename VertexElement>
  static auto end(const ElementsProvider* provider,
                  const VertexElement& element)
      -> decltype(provider->end(element)) {
    return provider->end(element);
  }

 private:
  const ElementsProvider* m_provider;
  std::tuple<ElementTypes...> m_elements;
  // std::tuple<VertexElementBufferProvider<ElementsProvider, ElementTypes>...>
  //    m_bufferProviders;
};

template <typename EP, typename... VE>
struct VertexBufferProviderTraits<TupleBufferDataProvider<EP, VE...>> {
 public:
  static const size_t vertexStart = 0;
  static const size_t maxBound = EP::HINT_MAX_BOUND;
  static const bool useIndexes = 0;
  using ProvidedElement = std::tuple<VE...>;
};

// TODO msati3: Finish implementation
/*
// Takes a collection of VertexElementBuffer providers, and provides for
// visitation over each of them.
template <typename... ElementBufferProviders>
class ChainedBufferDataProvider {
 public:
  using ProvidedElement = std::tuple<ElementBufferProviders...>;

  void begin(

 private:
  std::tuple<Providers...> m_vertexElementBufferProviders;
};*/

#endif  //_FRAMEWORK_RENDERING_TUPLE_ELEMENT_BUFFER_PROVIDER_H_
