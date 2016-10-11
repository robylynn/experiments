#ifndef _FRAMEWORK_RENDERING_VERTEXDATAIMPL_H_
#define _FRAMEWORK_RENDERING_VERTEXDATAIMPL_H_

#include "impl/vertexElementImpl.h"

namespace impl {
// Populate vertex buffer data. Set the vertex count to maxBound when reserving
// memory on the GPU.
template <typename DataProviderParams>
void createVertexData(Ogre::VertexData** vertexData) {
  *vertexData = OGRE_NEW Ogre::VertexData();
  (*vertexData)->vertexStart = DataProviderParams::vertexStart;
  (*vertexData)->vertexCount = DataProviderParams::maxBound;

  // Create vertex elements (attributes) for the types required
  for (auto vertexElement : DataProviderParams::vertexElements) {
    CreateVertexElementVisitor createVisitor(*vertexData);
    boost::apply_visitor(createVisitor, vertexElement);
  }
}

// Use a vertex buffer data provider that provides data for each of the
// elements (attributes) of the vertex buffer to fill up the vertex buffer's
// memory contents
template <typename DataProvider>
void populateVertexData(Ogre::VertexData* vertexData,
                        const DataProvider& provider) {
  vertexData->vertexCount = provider.size();
  for (auto vertexElement : DataProvider::Params::vertexElements) {
    PopulateVertexElementDataVisitor<DataProvider> populateVisitor(vertexData,
                                                                   provider);
    boost::apply_visitor(populateVisitor, vertexElement);
  }
}
}  // end namespace impl

#endif  //_FRAMEWORK_RENDERING_VERTEXDATAIMPL_H_
