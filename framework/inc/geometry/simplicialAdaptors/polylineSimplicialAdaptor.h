#ifndef _FRAMEWORK_GEOMETRY_POLYLINE_SIMPLICIAL_ADAPTOR_H_
#define _FRAMEWORK_GEOMETRY_POLYLINE_SIMPLICIAL_ADAPTOR_H_

#include <memory>

#include <containerAlgorithms.h>
#include "orderedCurveSimplicialAdaptorStrategy.h"

// A simplicial adaptor for the polyline representation. The polyline
// representation object itself must remain valid during the use of this
// adaptor class, and provide an iterator over vertices of the polyline.
template <typename LineType, typename SimplexType = LineList>
class PolylineSimplicialAdaptor
    : public OrderedCurveSimplicialAdaptorStrategy<SimplexType> {
 private:
  using AdaptorStrategy = OrderedCurveSimplicialAdaptorStrategy<SimplexType>;
  using LineIterator = typename LineType::const_iterator;
  const LineType* m_polyline;

 public:
  using const_iterator =
      utils::stencil_iterator<LineIterator, AdaptorStrategy::VERTICES_PER_BASE>;

  PolylineSimplicialAdaptor(const LineType& polyline) : m_polyline(&polyline) {}
  PolylineSimplicialAdaptor(const LineType&& polyline) = delete;
  ~PolylineSimplicialAdaptor() {}

  size_t size() const {
    return AdaptorStrategy::VERTICES_PER_BASE * m_polyline->size();
  }

  const_iterator begin() const {
    return const_iterator(m_polyline->begin(), AdaptorStrategy::stencil());
  }

  const_iterator end() const {
    return const_iterator(m_polyline->end() - 1, AdaptorStrategy::stencil());
  }
};

// A polyline simplicial adaptor is a lightweight object. So, we specialize the
// storage policy to by value. This allows for nicer client syntax through
// implicit temporary creation for PolylineSimplicialAdaptor.
template <typename ElementProvider, typename ElementType>
class ElementProviderStorageStrategy<
    PolylineSimplicialAdaptor<ElementProvider, ElementType>>
    : public CopyProviderStorageStrategy<
          PolylineSimplicialAdaptor<ElementProvider, ElementType>> {
 protected:
  using CopyProviderStorageStrategy<PolylineSimplicialAdaptor<
      ElementProvider, ElementType>>::CopyProviderStorageStrategy;
};

#endif  //_FRAMEWORK_GEOMETRY_POLYLINE_SIMPLICIAL_ADAPTOR_H_
