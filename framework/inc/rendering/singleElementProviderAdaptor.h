#ifndef _FRAMEWORK_RENDERING_SINGLE_ELEMENT_PROVIDER_ADAPTOR_H_
#define _FRAMEWORK_RENDERING_SINGLE_ELEMENT_PROVIDER_ADAPTOR_H_

#include <functional>

#include <boost/iterator/iterator_facade.hpp>

#include "renderingConstants.h"
#include "vertexElementBufferProvider.h"

// Adapts a class that provides begin and end over a single VertexElement type
// only to be a suitable vertex element buffer provider by providing begin and
// end wrappers that accept and VertexElement, and forwarding the calls to the
// VertexElement provider. This allows for simple STL containers to be used as
// VertexElement providers.
template <typename ElementProvider, typename ElementType>
class SingleElementProviderAdaptor
    : VertexElementProviderTraits<ElementProvider,
                                  ElementType>::storage_strategy {
  using StorageStrategy =
      typename VertexElementProviderTraits<ElementProvider,
                                           ElementType>::storage_strategy;
  using const_iterator =
      typename VertexElementProviderTraits<ElementProvider,
                                           ElementType>::const_iterator;

 public:
  SingleElementProviderAdaptor(const SingleElementProviderAdaptor& other)
      : StorageStrategy(*other.m_provider) {}

  template <class EP>
  SingleElementProviderAdaptor(EP&& provider)
      : StorageStrategy(std::forward<EP>(provider)) {}

  size_t size() const { return this->m_provider->size(); }

  const_iterator begin(const ElementType& element) const {
    return this->m_provider->begin();
  }

  const_iterator end(const ElementType& element) const {
    return this->m_provider->end();
  }
};

// A single element buffer provider adaptor specializes the storage policy to
// store by value. This allows for nicer client syntax through implicit
// SingleElementProviderAdaptor(ElementProvider) temporary creation, and
// binding of this temporary to VertexElementBufferProvider.
template <typename ElementProvider, typename ElementType>
class ElementProviderStorageStrategy<
    SingleElementProviderAdaptor<ElementProvider, ElementType>>
    : public CopyProviderStorageStrategy<
          SingleElementProviderAdaptor<ElementProvider, ElementType>> {
 protected:
  using CopyProviderStorageStrategy<SingleElementProviderAdaptor<
      ElementProvider, ElementType>>::CopyProviderStorageStrategy;
};

// Also specialize the VertexElementProvider traits for a
// SingleElementProviderAdaptor to conform to a pass through container
template <typename ElementProvider, typename ElementType>
struct VertexElementProviderTraits<
    SingleElementProviderAdaptor<ElementProvider, ElementType>, ElementType> {
  using provider_type =
      SingleElementProviderAdaptor<ElementProvider, ElementType>;
  using provided_type =
      typename VertexElementProviderTraits<ElementProvider,
                                           ElementType>::provided_type;
  using const_iterator = typename ElementProvider::const_iterator;
  using storage_strategy = ElementProviderStorageStrategy<
      SingleElementProviderAdaptor<ElementProvider, ElementType>>;
};

#endif  //_FRAMEWORK_RENDERING_SINGLE_ELEMENT_PROVIDER_ADAPTOR_H_
