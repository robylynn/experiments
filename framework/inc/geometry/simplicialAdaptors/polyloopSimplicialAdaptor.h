#ifndef _FRAMEWORK_GEOMETRY_POLYLOOP_SIMPLICIAL_ADAPTOR_H_
#define _FRAMEWORK_GEOMETRY_POLYLOOP_SIMPLICIAL_ADAPTOR_H_

#include <memory>

#include <CGAL/circulator.h>

#include <containerAlgorithms.h>
#include "orderedCurveSimplicialAdaptorStrategy.h"

// A simplicial adaptor for the polyloop representation. The polyloop
// representation object itself must remain valid during the use of this
// adaptor class, and provide an iterator over vertices of the loop. The first
// and last points are connected by the simplicial adaptor.
template <typename LoopType, typename SimplexType = LineList>
class PolyloopSimplicialAdaptor
    : public OrderedCurveSimplicialAdaptorStrategy<SimplexType> {
 private:
  using AdaptorStrategy = OrderedCurveSimplicialAdaptorStrategy<SimplexType>;
  using LoopCirculator =
      CGAL::Circulator_from_iterator<typename LoopType::const_iterator>;
  using CirculatorContainer = CGAL::Container_from_circulator<LoopCirculator>;
  using LoopIterator = decltype(std::declval<CirculatorContainer&>().begin());

  const LoopType* m_polyloop;
  LoopCirculator m_circulator;
  CirculatorContainer m_circularContainer;

 public:
  using const_iterator =
      utils::stencil_circulator_iterator<LoopIterator,
                                         AdaptorStrategy::VERTICES_PER_BASE>;

  PolyloopSimplicialAdaptor(const LoopType& polyloop)
      : m_polyloop(&polyloop),
        m_circulator(m_polyloop->begin(), m_polyloop->end()),
        m_circularContainer(m_circulator) {}
  PolyloopSimplicialAdaptor(const LoopType&& polyloop) = delete;
  ~PolyloopSimplicialAdaptor() {}

  PolyloopSimplicialAdaptor(const PolyloopSimplicialAdaptor& provider) {
    m_polyloop = provider.m_polyloop;
    m_circulator = LoopCirculator(m_polyloop->vertices_begin(),
                                  m_polyloop->vertices_end());
    m_circularContainer = CirculatorContainer(m_circulator);
  }

  PolyloopSimplicialAdaptor(PolyloopSimplicialAdaptor&& provider)
      : m_polyloop(provider.m_polyloop),
        m_circulator(m_polyloop->vertices_begin(), m_polyloop->vertices_end()),
        m_circularContainer(m_circulator) {}

  size_t size() const {
    return AdaptorStrategy::VERTICES_PER_BASE * m_polyloop->size();
  }

  const_iterator begin() const {
    return const_iterator(m_circularContainer.begin(),
                          AdaptorStrategy::stencil());
  }

  const_iterator end() const {
    return const_iterator(m_circularContainer.end(),
                          AdaptorStrategy::stencil());
  }
};

/*
// A polyloop simplicial adaptor is a lightweight object. So, we specialize the
// storage policy to by value. This allows for nicer client syntax through
// implicit temporary creation for PolylopSimplicialAdaptor.
template <typename ElementProvider, typename ElementType>
class ElementProviderStorageStrategy<
    PolyloopSimplicialAdaptor<ElementProvider, ElementType>>
    : public CopyProviderStorageStrategy<
          PolyloopSimplicialAdaptor<ElementProvider, ElementType>> {
 protected:
  using CopyProviderStorageStrategy<PolyloopSimplicialAdaptor<
      ElementProvider, ElementType>>::CopyProviderStorageStrategy;
};*/

#endif  //_FRAMEWORK_GEOMETRY_POLYLOOP_SIMPLICIAL_ADAPTOR_H_
