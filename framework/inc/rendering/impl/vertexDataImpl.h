#ifndef _FRAMEWORK_RENDERING_VERTEXDATAIMPL_H_
#define _FRAMEWORK_RENDERING_VERTEXDATAIMPL_H_

#include "containerAlgorithms.h"
#include "impl/vertexElementImpl.h"

template <typename BufferProvider>
struct VertexBufferProviderTraits {};

namespace impl {
// Populate vertex buffer data. Set the vertex count to maxBound when reserving
// memory on the GPU.
template <typename BufferProvider>
void createVertexData(Ogre::VertexData** vertexData) {
  using ProviderTraits = VertexBufferProviderTraits<BufferProvider>;

  *vertexData = OGRE_NEW Ogre::VertexData();
  (*vertexData)->vertexStart = ProviderTraits::vertexStart;
  (*vertexData)->vertexCount = ProviderTraits::maxBound;

  CreateVertexElementVisitor createVisitor(*vertexData);
  utils::for_each(typename ProviderTraits::ProvidedElements(), createVisitor);
}

// Use a vertex buffer data provider that provides data for each of the
// elements (attributes) of the vertex buffer to fill up the vertex buffer's
// memory contents
template <typename BufferProvider>
void populateVertexData(Ogre::VertexData* vertexData,
                        const BufferProvider& provider) {
  vertexData->vertexCount = provider.size();
  PopulateVertexElementDataVisitor<BufferProvider> populateVisitor(vertexData,
                                                                   provider);
  utils::for_each(
      typename VertexBufferProviderTraits<BufferProvider>::ProvidedElements(),
      populateVisitor);
}

}  // end namespace impl

#endif  //_FRAMEWORK_RENDERING_VERTEXDATAIMPL_H_
