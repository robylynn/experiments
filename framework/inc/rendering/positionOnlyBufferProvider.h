#ifndef _FRAMEWORK_RENDERING_POSITIONONLYBUFFERPROVIDER_H_
#define _FRAMEWORK_RENDERING_POSITIONONLYBUFFERPROVIDER_H_

#include <functional>

#include <boost/iterator/iterator_facade.hpp>

#include "renderingConstants.h"
#include "vertexElement.h"
#include "vertexData.h"

// Adapts a class that provides begin and end over geometry only to be a
// suitable vertex element data provider by forwarding requests for begin and
// end iterators for geometry data to the parameterless begin and end of the
// SequentialGeometryProvider. The SequentialGeometryProvider must provide type
// const_iterator over its contents, and of the returned iterators should be
// indexable from 0 to NUM_POSITION_COORDS -- TODO msati3: This could be relaxed
// to only be iterable.
//
// The position only provider demands the following concepts from the
// SequentialGeometryProvider:
// cost_iterator typedef
// Additionally, if the provider object is passed in itself, instead of the
// provider iterators, the provider must also provide the following:
// begin and end (return type = const_iterator)
template <typename SequentialGeometryProvider>
class PositionOnlyBufferProvider {
 public:
  PositionOnlyBufferProvider() : m_provider(nullptr) {}

  // Use this overload to pre-populate sequential geometry provider, but use
  // begin-end as the iteration functions (useful when the geometry provider is
  // a stl container)
  PositionOnlyBufferProvider(const SequentialGeometryProvider& geometryProvider)
      : m_provider(&geometryProvider),
        m_beginFn(
            std::bind(&SequentialGeometryProvider::begin, &geometryProvider)),
        m_endFn(std::bind(&SequentialGeometryProvider::end)) {}

  // Use to provide own sequential geometry provider, as well as functions that
  // return begin and end iterators over the geometry provider's geometry.
  PositionOnlyBufferProvider(
      const SequentialGeometryProvider& geometryProvider,
      std::function<typename SequentialGeometryProvider::const_iterator()>
          beginFn,
      std::function<typename SequentialGeometryProvider::const_iterator()>
          endFn)
      : m_provider(geometryProvider), m_beginFn(beginFn), m_endFn(endFn) {}

  // Convenience wrapper over member functions for begin and end
  PositionOnlyBufferProvider(
      const SequentialGeometryProvider& geometryProvider,
      std::function<typename SequentialGeometryProvider::const_iterator(
          const SequentialGeometryProvider*)> beginFn,
      std::function<typename SequentialGeometryProvider::const_iterator(
          const SequentialGeometryProvider*)> endFn)
      : m_provider(geometryProvider),
        m_beginFn(std::bind(beginFn, geometryProvider)),
        m_endFn(std::bind(endFn, geometryProvider)) {}

  PositionOnlyBufferProvider(
      const SequentialGeometryProvider&& geometryProvider) = delete;

  class CoordinateIterator
      : public boost::iterator_facade<CoordinateIterator, float,
                                      boost::forward_traversal_tag, float> {
   public:
    CoordinateIterator(const PositionOnlyBufferProvider* positionProvider,
                       typename SequentialGeometryProvider::const_iterator
                           sequentialProviderIter,
                       int coordinateIndex)
        : m_positionProvider(positionProvider),
          m_sequentialProviderIter(sequentialProviderIter),
          m_coordinateIndex(coordinateIndex) {}

   private:
    friend class boost::iterator_core_access;

    void increment() {
      if (++m_coordinateIndex == NUM_POSITION_COORDS) {
        m_coordinateIndex = 0;
        ++m_sequentialProviderIter;
      }
    }

    bool equal(const CoordinateIterator& other) const {
      return (m_coordinateIndex == other.m_coordinateIndex) &&
             (m_sequentialProviderIter == other.m_sequentialProviderIter) &&
             (m_positionProvider == other.m_positionProvider);
    }

    float dereference() const {
      return (*m_sequentialProviderIter)[m_coordinateIndex];
    }

    const PositionOnlyBufferProvider* m_positionProvider;
    typename SequentialGeometryProvider::const_iterator
        m_sequentialProviderIter;
    int m_coordinateIndex;
  };

  // Allow for querying size of provider
  size_t size(const SequentialGeometryProvider& provider) const {
    return NUM_POSITION_COORDS * provider.size();
  }

  size_t size() const {
    assert(m_provider != nullptr);
    return NUM_POSITION_COORDS * m_provider->size();
  }

  // Accept just the vertexElement, and forward calls using stored instance
  CoordinateIterator begin(const PositionVertexElement& vertexElement) const {
    assert(m_provider != nullptr);
    return begin(*m_provider, vertexElement);
  }

  CoordinateIterator end(const PositionVertexElement& vertexElement) const {
    assert(m_provider != nullptr);
    return end(*m_provider, vertexElement);
  }

  // Accept the provider itself, and use stored m_begin and m_end to forward
  CoordinateIterator begin(const SequentialGeometryProvider& provider,
                           const PositionVertexElement& vertexElement) const {
    return begin(m_beginFn(), vertexElement);
  }

  CoordinateIterator end(const SequentialGeometryProvider& provider,
                         const PositionVertexElement& vertexElement) const {
    return end(m_endFn(), vertexElement);
  }

  // Accept iterators to the provider begin and end, allowing for non-standard
  // names to the iterators being used
  CoordinateIterator begin(
      typename SequentialGeometryProvider::const_iterator providerBegin,
      const PositionVertexElement& /*vertexElement*/) const {
    return CoordinateIterator(this, providerBegin, 0);
  }

  CoordinateIterator end(
      typename SequentialGeometryProvider::const_iterator providerEnd,
      const PositionVertexElement& /*vertexElement*/) const {
    return CoordinateIterator(this, providerEnd, 0);
  }

  using const_iterator = CoordinateIterator;
  using GeometryProvider = SequentialGeometryProvider;
  using Params = VertexBufferDataProviderParams<
      PositionOnlyBufferProvider<GeometryProvider>>;

 private:
  const SequentialGeometryProvider* m_provider;
  std::function<typename SequentialGeometryProvider::const_iterator()> m_beginFn;
  std::function<typename SequentialGeometryProvider::const_iterator()> m_endFn;
};

template <typename GP>
class VertexBufferDataProviderParams<PositionOnlyBufferProvider<GP>> {
 public:
  static const size_t vertexStart = 0;
  static const size_t maxBound = GP::HINT_MAX_BOUND;
  static const bool useIndexes = 0;
  static std::vector<VertexElementsVariant> vertexElements;
};

template <typename GP>
std::vector<VertexElementsVariant> VertexBufferDataProviderParams<
    PositionOnlyBufferProvider<GP>>::vertexElements = {PositionVertexElement()};

#endif  //_FRAMEWORK_RENDERING_POSITIONONLYBUFFERPROVIDER_H__
