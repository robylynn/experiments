#ifndef _FRAMEWORK_GEOMETRY_DISC_SIMPLICIAL_ADAPTOR_H_
#define _FRAMEWORK_GEOMETRY_DISC_SIMPLICIAL_ADAPTOR_H_

#include <memory>

#include <containerAlgorithms.h>
#include "simplicialAdaptorStrategy.h"

template <>
class SimplicialAdaptorStrategy<DiscSimplicialAdaptor, TriangleFan> {
}

// A simplicial adaptor for a topological disc. The disc data representation
// itself must remain valid during the use of this adaptor class, and provide
// an iterator that first provides the value associated with the  center of the
// disc, and then, those associated with ordered samples on the disc.
template <typename DiscRep, typename SimplexType = TriangleFan>
class DiscSimplicialAdaptor : public SimplicialAdaptorStrategy<SimplexType> {
 private:
  using DiscIterator = typename DiscRep::const_iterator;

  const DiscRep* m_disc;

 public:
  using const_iterator = DiscRep::const_iterator;

  DiscSimplicialAdaptor(const DiscType& disc) : m_disc(&disc) {}
  DiscSimplicialAdaptor(const DiscType&& disc) = delete;
  ~DiscSimplicialAdaptor() {}

  size_t size() const { return m_disc->size(); }
  const_iterator begin() const { return m_disc->begin(); }
  const_iterator end() const { return m_disc->end(); }
};

// A disc simplicial adaptor is a lightweight adaptor. So, we specialize the
// storage policy to by value. This allows for nicer client syntax through
// implicit temporary creation for DiscSimplicialAdaptor.
template <typename ElementProvider, typename ElementType>
class ElementProviderStorageStrategy<
    DiscSimplicialAdaptor<ElementProvider, ElementType>>
    : public CopyProviderStorageStrategy<
          DiscSimplicialAdaptor<ElementProvider, ElementType>> {
 protected:
  using CopyProviderStorageStrategy<DiscSimplicialAdaptor<
      ElementProvider, ElementType>>::CopyProviderStorageStrategy;
};

#endif  //_FRAMEWORK_GEOMETRY_DISC_SIMPLICIAL_ADAPTOR_H_
