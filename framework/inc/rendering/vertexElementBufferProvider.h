#ifndef _FRAMEWORK_RENDERING_VERTEX_ELEMENT_BUFFER_PROVIDER_H_
#define _FRAMEWORK_RENDERING_VERTEX_ELEMENT_BUFFER_PROVIDER_H_

#include <functional>

#include <boost/iterator/iterator_facade.hpp>

#include "renderingConstants.h"
#include "vertexElement.h"
#include "vertexData.h"

// Default storage strategy for element providers is pointer storage
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
template <typename ElementProvider, typename VertexElement,
          typename StorageStrategy =
              ElementProviderStorageStrategy<ElementProvider>>
class VertexElementBufferProvider : public StorageStrategy {
  using ElementIterator =
      typename VertexElementProviderTraits<ElementProvider,
                                           VertexElement>::const_iterator;

 public:
  // Most generic form of buffer provider. Allows customization of begin and
  // end.
  VertexElementBufferProvider(const ElementProvider& provider,
                              const std::function<ElementIterator()>& beginFn,
                              const std::function<ElementIterator()>& endFn)
      : StorageStrategy(provider), m_beginFn(beginFn), m_endFn(endFn) {}

  // Convenience wrapper over member functions for begin and end
  VertexElementBufferProvider(
      const ElementProvider& provider,
      const std::function<ElementIterator(const ElementProvider*,
                                          const VertexElement&)>& beginFn =
          &ElementProvider::begin,
      const std::function<ElementIterator(const ElementProvider*,
                                          const VertexElement&)> endFn =
          &ElementProvider::end)
      : StorageStrategy(provider),
        m_beginFn(std::bind(&ElementProvider::begin, this->m_provider,
                            VertexElement())),
        m_endFn(std::bind(&ElementProvider::end, this->m_provider,
                          VertexElement())) {}

  template <typename ElementIter = ElementIterator>
  class CoordinateIterator
      : public boost::iterator_facade<CoordinateIterator<ElementIter>, float,
                                      boost::forward_traversal_tag, float> {
   public:
    CoordinateIterator(const VertexElementBufferProvider* provider,
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
      return (*m_providerIter)[m_coordinateIndex];
    }

    const VertexElementBufferProvider* m_provider;
    ElementIter m_providerIter;
    int m_coordinateIndex;
  };

  size_t size() const {
    assert(this->m_provider != nullptr);
    return VertexElement::elementSize * this->m_provider->size();
  }

  // Accept just the vertexElement, and forward calls using stored instance
  CoordinateIterator<ElementIterator> begin() const {
    assert(this->m_provider != nullptr);
    return begin(*(this->m_provider));
  }

  CoordinateIterator<ElementIterator> end() const {
    assert(this->m_provider != nullptr);
    return end(*(this->m_provider));
  }

  using const_iterator = CoordinateIterator<ElementIterator>;

 private:
  CoordinateIterator<ElementIterator> begin(
      const ElementProvider& provider) const {
    return begin(m_beginFn());
  }

  CoordinateIterator<ElementIterator> end(
      const ElementProvider& provider) const {
    return end(m_endFn());
  }

  // Accept iterators to the provider begin and end, allowing for non-standard
  // names to the iterators being used
  CoordinateIterator<ElementIterator> begin(
      ElementIterator providerBegin) const {
    return CoordinateIterator<ElementIterator>(this, providerBegin, 0);
  }

  CoordinateIterator<ElementIterator> end(ElementIterator providerEnd) const {
    return CoordinateIterator<ElementIterator>(this, providerEnd, 0);
  }

  std::function<ElementIterator(void)> m_beginFn;
  std::function<ElementIterator(void)> m_endFn;
};

template <typename EP, typename VE>
struct VertexBufferDataProviderParams<VertexElementBufferProvider<VE, EP>> {
 public:
  static const size_t vertexStart = 0;
  static const size_t maxBound = EP::HINT_MAX_BOUND;
  static const bool useIndexes = 0;
  using ProvidedElements = std::tuple<VE>;
};

#endif  //_FRAMEWORK_RENDERING_VERTEX_ELEMENT_BUFFER_PROVIDER_H_
