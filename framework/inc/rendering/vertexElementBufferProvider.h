#ifndef _FRAMEWORK_RENDERING_VERTEX_ELEMENT_BUFFER_PROVIDER_H_
#define _FRAMEWORK_RENDERING_VERTEX_ELEMENT_BUFFER_PROVIDER_H_

#include <functional>

#include <boost/iterator/iterator_facade.hpp>

#include "renderingConstants.h"
#include "vertexElement.h"
#include "vertexElementProviderTraits.h"
#include "vertexBufferProviderTraits.h"
#include "vertexElementIndexer.h"

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
          typename StorageStrategy = typename VertexElementProviderTraits<
              ElementProvider, VertexElement>::storage_strategy>
class VertexElementBufferProvider : public StorageStrategy {
  using ElementIterator =
      typename VertexElementProviderTraits<ElementProvider,
                                           VertexElement>::const_iterator;

 public:
  // Accept and store element provider.
  VertexElementBufferProvider(const ElementProvider& provider)
      : StorageStrategy(provider) {}

  template <typename ElementIter = ElementIterator>
  class CoordinateIterator
      : public boost::iterator_facade<
            CoordinateIterator<ElementIter>, typename VertexElement::AtomicType,
            boost::forward_traversal_tag, typename VertexElement::AtomicType> {
   public:
    CoordinateIterator(const VertexElementBufferProvider* provider,
                       const ElementIter& providerIter, int coordinateIndex)
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
      return static_cast<typename VertexElement::AtomicType>(
          VertexElementIndexer<
              const typename std::iterator_traits<ElementIter>::reference>()(
              *m_providerIter, m_coordinateIndex));
    }

    const VertexElementBufferProvider* m_provider;
    ElementIter m_providerIter;
    int m_coordinateIndex;
  };

  size_t size() const {
    assert(this->m_provider != nullptr);
    return VertexElement::elementSize * this->m_provider->size();
  }

  // Accept nothing -- the VertexElement is implicitly known
  CoordinateIterator<ElementIterator> begin() const {
    assert(this->m_provider != nullptr);
    return begin(this->m_provider, m_vertexElement);
  }

  CoordinateIterator<ElementIterator> end() const {
    assert(this->m_provider != nullptr);
    return end(this->m_provider, m_vertexElement);
  }

  // Accept a VertexElement -- this allows VertexElementBuffer provider to be
  // used as a VertexDataBufferProvider (one that provides VertexBufferData for
  // a single VertexElement)
  CoordinateIterator<ElementIterator> begin(
      const VertexElement& vertexElement) const {
    assert(this->m_provider != nullptr);
    return begin(this->m_provider, vertexElement);
  }

  CoordinateIterator<ElementIterator> end(
      const VertexElement& vertexElement) const {
    assert(this->m_provider != nullptr);
    return end(this->m_provider, vertexElement);
  }

  using const_iterator = CoordinateIterator<ElementIterator>;

 private:
  CoordinateIterator<ElementIterator> begin(
      const ElementProvider* provider,
      const VertexElement& vertexElement) const {
    return begin(provider->begin(vertexElement));
  }

  CoordinateIterator<ElementIterator> end(
      const ElementProvider* provider,
      const VertexElement& vertexElement) const {
    return end(provider->end(vertexElement));
  }

  // Accept iterators to the provider begin and end, allowing for non-standard
  // names to the iterators being used
  CoordinateIterator<ElementIterator> begin(
      const ElementIterator& providerBegin) const {
    return CoordinateIterator<ElementIterator>(this, providerBegin, 0);
  }

  CoordinateIterator<ElementIterator> end(
      const ElementIterator& providerEnd) const {
    return CoordinateIterator<ElementIterator>(this, providerEnd, 0);
  }

  VertexElement m_vertexElement;
};

template <typename EP, typename VE>
struct VertexBufferProviderTraits<VertexElementBufferProvider<EP, VE>> {
  static const size_t vertexStart = 0;
  static const size_t maxBound =
      VertexElementProviderTraits<EP, VE>::provided_type::HINT_MAX_BOUND;
  static const bool useIndexes = 0;

  using vertex_elements = std::tuple<VE>;
  using element_providers = std::tuple<EP>;
};

#endif  //_FRAMEWORK_RENDERING_VERTEX_ELEMENT_BUFFER_PROVIDER_H_
