#ifndef _FRAMEWORK_RENDERING_POSITIONONLY_PROVIDER_H_
#define _FRAMEWORK_RENDERING_POSITIONONLY_PROVIDER_H_

#include <boost/iterator/iterator_facade.hpp>

#include "vertexElement.h"

// Adapts a class that provides begin and end over geometry only to be a
// suitable vertex element data provider by forwarding requests for begin and
// end iterators for geometry data to the parameterless begin and end of the
// SequentialGeometryProvider. The SequentialGeometryProvider must provide type
// const_iterator over its contents, and of the returned iterators should be
// indexable from 0 to 3 -- TODO msati3: This could be relaxed to only be
// iterable.
//
// The position only provider demans the following concepts:
// begin, end, size and cost_iterator typedef
template <typename SequentialGeometryProvider>
class PositionOnlyProvider {
 public:
  PositionOnlyProvider(const SequentialGeometryProvider& provider)
      : m_provider(&provider) {}
  PositionOnlyProvider(const SequentialGeometryProvider&& provider) = delete;

  class CoordinateIterator
      : public boost::iterator_facade<CoordinateIterator, float,
                                      boost::forward_traversal_tag, float> {
   public:
    CoordinateIterator(const PositionOnlyProvider* positionProvider,
                       typename SequentialGeometryProvider::const_iterator
                           sequentialProviderIter,
                       int coordinateIndex)
        : m_positionProvider(positionProvider),
          m_sequentialProviderIter(sequentialProviderIter),
          m_coordinateIndex(coordinateIndex) {}

   private:
    friend class boost::iterator_core_access;

    void increment() {
      ++m_coordinateIndex;
      if (m_coordinateIndex == 3) {
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

    const PositionOnlyProvider* m_positionProvider;
    typename SequentialGeometryProvider::const_iterator
        m_sequentialProviderIter;
    int m_coordinateIndex;
  };

  size_t size() { return m_provider->size(); }

  CoordinateIterator begin(
      const PositionVertexElement& /*vertexElement*/) const {
    return CoordinateIterator(this, m_provider->begin(), 0);
  }

  CoordinateIterator end(const PositionVertexElement& /*vertexElement*/) const {
    return CoordinateIterator(this, m_provider->end(), 3);
  }

  using const_iterator = CoordinateIterator;

 private:
  const SequentialGeometryProvider* m_provider;
};

#endif  //_FRAMEWORK_RENDERING_POSITIONONLY_PROVIDER_H__
