#ifndef _FRAMEWORK_RENDERING_VERTEX_ELEMENTS_BUFFER_PROVIDER_H_
#define _FRAMEWORK_RENDERING_VERTEX_ELEMENTS_BUFFER_PROVIDER_H_

#include <functional>

#include <boost/iterator/iterator_facade.hpp>

#include "attributes/vertexAttributesProviderTraits.h"
#include "renderingConstants.h"
#include "vertexElement.h"
#include "vertexBufferProviderTraits.h"

/** A vertex attribute is any per vertex attribute (position, color,
 * etc). A vertex attribute is a n-dimensional entity. The vertex element buffer
 * provider adapts a vertex attribute provider by "unwrapping" the attributes
 * and providing iteration over each dimension of a particular vertex attribute
 * for each vertex.

 * Thus, it adapts a class that provides iterations over a contained vertex
 * attribute set to be a suitable vertex data provider for a particular vertex
 * element by forwarding requests for begin and end iterators to the
 * VertexAttributeProvider, dereferencing the resulting information, and, if
 * it is a tuple, obtaining the attribute corresponding to the VertexElement,
 * and then indexing into the returned value (which should be indexable from 0
 * to VertexElement::elementSize). In case the returned iterators are indexable
 * for only a subset of this range, the VertexAttributeProvider's data will be
 * extended with the defaultExtension value for the VertexElement associated
 * wit the AttributeType that the VertexAttributeProvider is providing.
 *
 * The vertex element buffer provider demands the following traits from the
 * VertexAttributeProvider: const_iterator -- an iterator over the vertices.
 * Additionally, if the provider object is passed in itself, instead of the
 * provider iterators, the provider must also provide the following: begin and
 * end (return type = const_iterator), that also accept a AttributeType as
 * parameter, and yield an iterator that may be dereferenced to obtain the
 * AttributeProvider's ProvidedElement type.
 */
template <typename VertexAttributesProvider, typename... VertexAttributes>
class VertexElementsBufferProvider
    : public typename VertexAttributesProviderTraits<
          VertexAttributesProvider>::storage_strategy {
  using AttributesIterator = typename VertexAttributesProviderTraits<
      VertexAttributesProvider>::const_iterator;

 public:
  using VertexElements = vertex_element_for_attributes_t<VertexAttributes>;

  // Accept and store element provider.
  VertexElementBufferProvider(const VertexAttributesProvider& provider)
      : StorageStrategy(provider) {}

  template <typename VertexElement, typename AttribsIter = AttributesIterator>
  class CoordinateIterator
      : public boost::iterator_facade<
            CoordinateIterator<AttribsIter>, typename VertexElement::AtomicType,
            boost::forward_traversal_tag, typename VertexElement::AtomicType> {
   public:
    CoordinateIterator(const VertexElementBufferProvider* provider,
                       const AttribsIter& attribsIter, int coordinateIndex)
        : m_provider(provider),
          m_attribsIter(attribsIter),
          m_coordinateIndex(coordinateIndex) {}

   private:
    friend class boost::iterator_core_access;

    void increment() {
      if (++m_coordinateIndex == VertexElement::elementSize) {
        m_coordinateIndex = 0;
        ++m_attribsIter;
      }
    }

    bool equal(const CoordinateIterator& other) const {
      return (m_coordinateIndex == other.m_coordinateIndex) &&
             (m_attribsIter == other.m_attribsIter) &&
             (m_provider == other.m_provider);
    }

    // Forward to dereference delegate to allow being overloaded for different
    // attribute types. (This is useful in case some attribute types don't
    // provide [] indexing operator)
    typename VertexElement::AtomicType dereference() const {
      return static_cast<typename VertexElement::AtomicType>(
          AttributeTypeDimensionIndexer<typename VertexAttribute::type>()(
              std::get<typename VertexAttribute::type>(*m_attribsIter),
              m_coordinateIndex));
    }

    const VertexElementBufferProvider* m_provider;
    AttribsIter m_attribsIter;
    int m_coordinateIndex;
  };

  size_t size() const {
    assert(this->m_provider != nullptr);
    return VertexElement::elementSize * this->m_provider->size();
  }

  // Accept nothing-- the VertexElement is implicitly known
  CoordinateIterator<AttributesIterator> begin() const {
    assert(this->m_provider != nullptr);
    return begin(this->m_provider, m_vertexElement);
  }

  CoordinateIterator<AttributesIterator> end() const {
    assert(this->m_provider != nullptr);
    return end(this->m_provider, m_vertexElement);
  }

  // Accept a VertexElement -- this allows VertexElementBuffer provider to be
  // used as a VertexDataBufferProvider (one that provides VertexBufferData for
  // a single VertexElement)
  CoordinateIterator<AttributesIterator> begin(
      const VertexElement& vertexElement) const {
    assert(this->m_provider != nullptr);
    return begin(this->m_provider, vertexElement);
  }

  CoordinateIterator<AttributesIterator> end(
      const VertexElement& vertexElement) const {
    assert(this->m_provider != nullptr);
    return end(this->m_provider, vertexElement);
  }

  using const_iterator = CoordinateIterator<AttributesIterator>;

 private:
  CoordinateIterator<AttributesIterator> begin(
      const VertexAttributesProvider* provider,
      const VertexElement& vertexElement) const {
    return begin(provider->begin(vertexElement));
  }

  CoordinateIterator<AttributesIterator> end(
      const VertexAttributesProvider* provider,
      const VertexElement& vertexElement) const {
    return end(provider->end(vertexElement));
  }

  // Accept iterators to the provider begin and end, allowing for non-standard
  // names to the iterators being used.
  CoordinateIterator<AttributesIterator> begin(
      const AttributesIterator& providerBegin) const {
    return CoordinateIterator<AttributesIterator>(this, providerBegin, 0);
  }

  CoordinateIterator<AttributesIterator> end(
      const AttributesIterator& providerEnd) const {
    return CoordinateIterator<AttributesIterator>(this, providerEnd, 0);
  }

  VertexElement m_vertexElement;
};

template <typename AttributesProvider, typename VertexAttribute>
struct VertexBufferProviderTraits<
    VertexElementBufferProvider<AttributesProvider>> {
  static const size_t vertexStart = 0;
  static const size_t maxBound = 10000;
  static const bool useIndexes = 0;

  using vertex_elements = std::tuple<VA>;
  using element_providers = std::tuple<VA>;
};

#endif  //_FRAMEWORK_RENDERING_VERTEX_ELEMENT_BUFFER_PROVIDER_H_
