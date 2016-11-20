#ifndef _FRAMEWORK_GEOMETRY_POLYLINE_SIMPLICIAL_ADAPTOR_H_
#define _FRAMEWORK_GEOMETRY_POLYLINE_SIMPLICIAL_ADAPTOR_H_

#include <memory>

#include <containerAlgorithms.h>
#include "attributes/vertexAttributeProviderTraits.h"
#include "orderedCurveSimplicialAdaptorStrategy.h"

/* A simplicial adaptor for the polyline representation. The polyline
* representation object itself must remain valid during the use of this
* adaptor class, and provide an iterator over vertices of the polyline.
*/
template <typename LineRep, typename SimplexType = LineList>
class PolylineSimplicialAdaptor
    : public OrderedCurveSimplicialAdaptorStrategy<SimplexType> {
 private:
  using AdaptorStrategy = OrderedCurveSimplicialAdaptorStrategy<SimplexType>;
  using LineIterator = typename LineRep::const_iterator;
  const LineRep* m_polyline;

 public:
  using const_iterator =
      utils::stencil_iterator<LineIterator, AdaptorStrategy::VERTICES_PER_BASE>;

  PolylineSimplicialAdaptor(const LineRep& polyline) : m_polyline(&polyline) {}
  PolylineSimplicialAdaptor(const LineRep&& polyline) = delete;
  ~PolylineSimplicialAdaptor() {}

  size_t size() const {
    return AdaptorStrategy::VERTICES_PER_BASE * m_polyline->vertices_size();
  }

  const_iterator begin() const {
    return const_iterator(m_polyline->vertices_begin(),
                          AdaptorStrategy::stencil());
  }

  const_iterator end() const {
    return const_iterator(m_polyline->vertices_end() - 1,
                          AdaptorStrategy::stencil());
  }
};

// A polyline simplicial adaptor is a lightweight object. So, we specialize the
// storage policy to by value. This allows for nicer client syntax through
// implicit temporary creation for PolylineSimplicialAdaptor.
template <typename LineRep, typename SimplexType>
class AttributeProviderStorageStrategy<
    PolylineSimplicialAdaptor<LineRep, SimplexType>>
    : public utils::CopyStorageStrategy<
          PolylineSimplicialAdaptor<LineRep, SimplexType>> {
 protected:
  using utils::CopyStorageStrategy<PolylineSimplicialAdaptor<
      AttributeProvider, AttributeType>>::CopyStorageStrategy;
};

template <typename LineRep, typename SimplexType>
struct DefaultSimplexType<PolylineSimplicialAdaptor<LineRep, SimplexType>> {
  using type = SimplexType;
};

#endif  //_FRAMEWORK_GEOMETRY_POLYLINE_SIMPLICIAL_ADAPTOR_H_
