#ifndef _FRAMEWORK_RENDERING_VERTEXDATAIMPL_H_
#define _FRAMEWORK_RENDERING_VERTEXDATAIMPL_H_

#include <experimental/tuple>

#include "impl/vertexElementImpl.h"

template <typename T>
struct VertexBufferDataProviderParams {};

namespace impl {
// Populate vertex buffer data. Set the vertex count to maxBound when reserving
// memory on the GPU.
template <typename DataProvider>
void createVertexData(Ogre::VertexData** vertexData) {
  using ProviderParams = VertexBufferDataProviderParams<DataProvider>;

  *vertexData = OGRE_NEW Ogre::VertexData();
  (*vertexData)->vertexStart = ProviderParams::vertexStart;
  (*vertexData)->vertexCount = ProviderParams::maxBound;

  CreateVertexElementVisitor createVisitor(*vertexData);
  std::experimental::apply(createVisitor, ProviderParams::ProvidedElements);
}

// Use a vertex buffer data provider that provides data for each of the
// elements (attributes) of the vertex buffer to fill up the vertex buffer's
// memory contents
template <typename DataProvider>
void populateVertexData(Ogre::VertexData* vertexData,
                        const DataProvider& provider) {
  vertexData->vertexCount = provider.size();
  PopulateVertexElementDataVisitor<DataProvider> populateVisitor(vertexData,
                                                                 provider);
  std::experimental::apply(populateVisitor,
             VertexBufferDataProviderParams<DataProvider>::ProvidedElements);
}

}  // end namespace impl

#endif  //_FRAMEWORK_RENDERING_VERTEXDATAIMPL_H_
