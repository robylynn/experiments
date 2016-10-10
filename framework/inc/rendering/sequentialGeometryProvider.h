#ifndef _SEQUENTIAL_GEOMETRY_PROVIDER_H_
#define _SEQUENTIAL_GEOMETRY_PROVIDER_H_

#include <boost/iterator/iterator_facade.hpp>

#include "vertexElement.h"

// Adapts a class that provides begin and end over geometry only to be a
// suitable vertex element data provider by forwarding requests for begin and
// end iterators for geometry data to the parameterless begin and end of the
// SequentialGeometryProvider. The SequentialGeometryProvider must provide type
// const_iterator over its contents, and of the returned iterators should be
// indexable from 0 to 3 -- TODO msati3: This could be relaxed to only be
// iterable.
template <typename SequentialGeometryProvider,
          typename SequentialGeometryProviderTraits>
class GeometryOnlyProvider3D : public static_visitor<void> {
 public:
  GeometryOnlyProvider(const SequentialGeometryProvider& provider)
      : m_provider(&provider) {}
  GeometryOnlyProvider(const SequentialGeometryProvider&& provider) = delete;

  class CoordinateIterator
      : public boost::iterator_facade<GeometryIterator, float,
                                      boost::forward_traversal_tag> {
    CoordinateIterator(
        const GeometryOnlyProvider* geometryOnlyProvider,
        SequentialGeometryProvider::const_iterator* sequentialProviderIter,
        int coordinateIndex)
        : m_geometryOnlyProvider(geometryOnlyProvider),
          m_sequentialProviderIter(sequentialProviderIter),
          m_coordinateIndex(coordinateIndex) {}

   private:
    friend class boost::iterator_core_access;

    void increment() {
      ++m_pointIndex;
      if (m_pointIndex == 3) {
        m_pointIndex = 0;
        ++m_sequentialProviderIterator;
      }
    }

    bool equal(const GeometryIterator& other) const {
      return (m_pointIndex == other.m_pointIndex) &&
             (m_sequentialProviderIter == other.m_sequentialProviderIter) &&
             (m_geometryOnlyProvider == other.m_geometryOnlyProvider);
    }

    float dereference() const {
      return (*m_sequentialProviderIterator)[m_pointIndex];
    }

    const GeometryOnlyProvider* m_geometryOnlyProvider;
    SequentialGeometryProvider::const_iterator m_sequentialProviderIterator;
    int m_pointIndex;
  };

  size_t size() { return m_provider->size(); }

  CoordinateIterator begin() const {
    return CoordinateIterator(this, m_provider->begin(), 0);
  }
  CoordinateIterator end() const {
    return CoordinateIterator(this, m_provider->end(), 3);
  }

  using const_iterator = CoordinateIterator;

 private:
  const SequentialGeometryProvider* m_provider;
};

#endif  //_SEQUENTIAL_GEOMETRY_PROVIDER_H_
