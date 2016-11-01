#ifndef _FRAMEWORK_RENDERING_SINGLE_ELEMENT_BUFFER_PROVIDER_H_
#define _FRAMEWORK_RENDERING_SINGLE_ELEMENT_BUFFER_PROVIDER_H_

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
class SingleElementBufferProviderAdaptor
    : VertexElementProviderTraits<ElementProvider,
                                  ElementType>::storage_strategy {
  using StorageStrategy =
      typename VertexElementProviderTraits<ElementProvider,
                                           ElementType>::storage_strategy;
  using const_iterator =
      typename VertexElementProviderTraits<ElementProvider,
                                           ElementType>::const_iterator;

 public:
  static constexpr Ogre::RenderOperation::OperationType PRIMITIVE_TYPE =
      ElementProvider::PRIMITIVE_TYPE;
  static constexpr int HINT_MAX_BOUND = ElementProvider::HINT_MAX_BOUND;

  SingleElementBufferProviderAdaptor(
      const SingleElementBufferProviderAdaptor& other)
      : StorageStrategy(*other.m_provider) {}

  template <class EP>
  SingleElementBufferProviderAdaptor(EP&& provider)
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
// SingleElementBufferProviderAdaptor(ElementProvider) temporary creation, and
// binding of this temporary to VertexElementBufferProvider.
template <typename ElementProvider, typename ElementType>
class ElementProviderStorageStrategy<
    SingleElementBufferProviderAdaptor<ElementProvider, ElementType>>
    : public CopyProviderStorageStrategy<
          SingleElementBufferProviderAdaptor<ElementProvider, ElementType>> {
 protected:
  using CopyProviderStorageStrategy<SingleElementBufferProviderAdaptor<
      ElementProvider, ElementType>>::CopyProviderStorageStrategy;
};

// Also specialize the VertexElementProvider traits for a
// SingleElementBufferProviderAdaptor to conform to a pass through container
template <typename ElementProvider, typename ElementType>
struct VertexElementProviderTraits<
    SingleElementBufferProviderAdaptor<ElementProvider, ElementType>,
    ElementType> {
  using provider_type =
      SingleElementBufferProviderAdaptor<ElementProvider, ElementType>;
  using const_iterator = typename ElementProvider::const_iterator;
  using storage_strategy = ElementProviderStorageStrategy<
      SingleElementBufferProviderAdaptor<ElementProvider, ElementType>>;
};

#endif  //_FRAMEWORK_RENDERING_SINGLE_ELEMENT_BUFFER_PROVIDER_H_
