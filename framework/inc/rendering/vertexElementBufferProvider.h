#ifndef _FRAMEWORK_RENDERING_VERTEX_ELEMENT_BUFFER_PROVIDER_H_
#define _FRAMEWORK_RENDERING_VERTEX_ELEMENT_BUFFER_PROVIDER_H_

#include <functional>

#include <boost/iterator/iterator_facade.hpp>

#include "renderingConstants.h"
#include "vertexElement.h"
#include "vertexData.h"

// Defitions: A vertex element is any per vertex attribute (position, color,
// etc). The vertex element is a n-dimensional entity. A vertex element
// provider provides iteration over all vertices for the vertex attribute.
//
// The vertex element buffer provider adapts the vertex element provider by
// "unwrapping" these elements and providing iteration over each dimension of
// the vertex element for each vertex.

// Adapts a class that provides iterations over a contained type to be a
// suitable vertex element data provider for a particular vertex element by
// forwarding requests for begin and end iterators to the ElementProvider,
// which could provide the type const_iterator over its contents, and the
// returned iterators should be indexable from 0 to VertexElement::elementSize.
// In case not, the ElementProvider will be extended with the defaultExtension
// value for the VertexElement that the ElementProvider is providing.
//
// The vertex element provider demands the following traits from the
// SequentialCollectionProvider: cost_iterator -- an iterator over the vertices
// of the SequentialCollection. Additionally, if the provider object is passed
// in itself, instead of the provider iterators, the provider must also provide
// the following: begin and end (return type = const_iterator), that also
// accept a VertexElement as parameter, and yield an iterator that may be
// dereferenced to obtain the VertexElement's ProvidedElement type.
template <typename VertexElement, typename ElementProvider>
class VertexElementBufferProvider {
  using ElementIterator =
      typename VertexElementProviderTraits<ElementProvider,
                                           VertexElement>::const_iterator;

 public:
  VertexElementBufferProvider() : m_provider(nullptr) {}

  // Most generic form of buffer provider. Allows customization of begin and
  // end, as well as providing a custom functor to apply to obtain resulting
  // value.
  VertexElementBufferProvider(
      const ElementProvider& provider,
      std::function<ElementIterator()> beginFn =
          std::bind(&ElementProvider::begin, &provider, VertexElement()),
      std::function<ElementIterator()> endFn = std::bind(&ElementProvider::end,
                                                         &provider,
                                                         VertexElement()),
      std::function<typename VertexElement::AtomicType(ElementIterator, int)
          coordinateFunctor = &ElementIterator::operator(int))
      : m_provider(provider),
        m_beginFn(beginFn),
        m_endFn(endFn),
        m_coordinateFunctor(coordinateFunctor) {}

  // Use this overload to pre-populate sequential collection provider, but use
  // begin-end as the iteration functions (useful when the collection provider
  // is a stl container)
  VertexElementBufferProvider(const ElementProvider& provider)
      : VertexElementBufferProvider(
            provider,
            std::bind(&ElementProvider::begin, &provider, VertexElement()),
            std::bind(&ElementProvider::end, &provider, VertexElement())) {}

  // Use this overload to pre-populate sequential collection provider, and
  // associate a coodinate access functor
  VertexElementBufferProvider(const ElementProvider& provider,
                              std::function<typename VertexElement::AtomicType(
                                  ElementIterator, int)> coordinateFunctor)
      : VertexElementBufferProvider(
            provider, std::bind(&ElementProvider::begin, &provider),
            std::bind(&ElementProvider::end, &provider), coordinateFunctor) {
  }

  // Convenience wrapper over member functions for begin and end
  VertexElementBufferProvider(
      const ElementProvider& provider,
      std::function<ElementIterator(const ElementProvider*,
                                    const VertexElement&)> beginFn,
      std::function<ElementIterator(const ElementProvider*,
                                    const VertexElement&)> endFn)
      : VertexElementBufferProvider(
            provider, std::bind(beginFn, provider, VertexElement()),
            std::bind(endFn, provider, VertexElement())) {}

  // The element provider should not be a temporary -- we expect it to remain
  // alive over the duration of iterating over the provided buffer.
  VertexElementBufferProvider(const ElementProvider&& provider) = delete;

  template <typename ElementIter = ElementIterator>
  class CoordinateIterator
      : public boost::iterator_facade<CoordinateIterator<ElementIter>, float,
                                      boost::forward_traversal_tag, float> {
   public:
    CoordinateIterator(const ElementProvider* provider,
                       ElementIter providerIter, int coordinateIndex)
        : m_provider(provider),
          m_providerIter(providerIter),
          m_coordinateIndex(coordinateIndex) {}

    CoordinateIterator(const ElementProvider* provider,
                       ElementIter providerIter, int coordinateIndex)
        : m_provider(provider),
          m_providerIter(providerIter),
          m_coordinateIndex(coordinateIndex) {}

   private:
    friend class boost::iterator_core_access;

    void increment() {
      if (++m_coordinateIndex == VertexElement::elementSize) {
        m_coordinateIndex = 0;
        ++m_providerIter;
      }
    }

    bool equal(const CoordinateIterator& other) const {
      return (m_coordinateIndex == other.m_coordinateIndex) &&
             (m_providerIter == other.m_providerIter) &&
             (m_provider == other.m_provider);
    }

    // Forward to dereference delegate to allow being overloaded based on
    // GeometryPointType
    typename VertexElement::AtomicType dereference() const {
      return m_coordinateFunctor(*m_providerIter, m_coordinateIndex);
    }

    const VertexElementBufferProvider* m_provider;
    ElementIter m_providerIter;
    int m_coordinateIndex;
  };

  // Allow for querying size of provider
  size_t size(const ElementProvider& provider) const {
    return VertexElement::elementSize * provider.size();
  }

  size_t size() const {
    assert(m_provider != nullptr);
    return VertexElement::elementSize * m_provider->size();
  }

  // Accept just the vertexElement, and forward calls using stored instance
  CoordinateIterator<ElementIterator> begin(
      const VertexElement& vertexElement) const {
    assert(m_provider != nullptr);
    return begin(*m_provider, vertexElement);
  }

  CoordinateIterator<ElementIterator> end(
      const VertexElement& vertexElement) const {
    assert(m_provider != nullptr);
    return end(*m_provider, vertexElement);
  }

  // Accept the provider itself, and use stored m_begin and m_end to forward
  CoordinateIterator<ElementIterator> begin(
      const SequentialGeometryProvider& provider,
      const VertexElement& vertexElement) const {
    return begin(m_beginFn(), vertexElement);
  }

  CoordinateIterator<ElementIterator> end(
      const SequentialGeometryProvider& provider,
      const VertexElement& vertexElement) const {
    return end(m_endFn(), vertexElement);
  }

  // Accept iterators to the provider begin and end, allowing for non-standard
  // names to the iterators being used
  CoordinateIterator<ElementIterator> begin(
      ElementIterator providerBegin,
      const VertexElement& /*vertexElement*/) const {
    return CoordinateIterator<ElementIterator>(this, providerBegin, 0);
  }

  CoordinateIterator<ElementIterator> end(
      ElementIterator providerEnd,
      const VertexElement& /*vertexElement*/) const {
    return CoordinateIterator<ElementIterator>(this, providerEnd, 0);
  }

  using const_iterator = CoordinateIterator<ElementIterator>;

 private:
  const ElementProvider* m_provider;
  std::function<ElementIterator()> m_beginFn;
  std::function<ElementIterator()> m_endFn;
  std::function<typename VertexElement::AtomicType(const ElementIterator*, int)>
      m_coordinateFunctor;
};

template <typename VE, typename EP>
struct VertexBufferDataProviderParams<VertexElementBufferProvider<VE, EP>> {
 public:
  static const size_t vertexStart = 0;
  static const size_t maxBound = EP::HINT_MAX_BOUND;
  static const bool useIndexes = 0;
  using ProvidedElement = VE;
};

#endif  //_FRAMEWORK_RENDERING_VERTEX_ELEMENT_BUFFER_PROVIDER_H_
