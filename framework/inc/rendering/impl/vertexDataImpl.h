#ifndef _FRAMEWORK_RENDERING_VERTEXDATAIMPL_H_
#define _FRAMEWORK_RENDERING_VERTEXDATAIMPL_H_

#include "impl/vertexElementImpl.h"

namespace impl {
// Populate vertex buffer data.
template <typename DataProviderParameters>
void createVertexData(Ogre::VertexData** vertexData) {
  *vertexData = OGRE_NEW Ogre::VertexData();
  (*vertexData)->vertexStart = DataProviderParameters::vertexStart;
  (*vertexData)->vertexCount = DataProviderParameters::vertexHint;

  // Create vertex elements (attributes) for the types required
  for (auto vertexElement : DataProviderParameters::vertexElements) {
    boost::apply_visitor(CreateVertexElementVisitor(*vertexData),
                         vertexElement);
  }
}

// Use a vertex buffer data provider that provides data for each of the
// elements (attributes) of the vertex buffer to fill up the vertex buffer's
// memory contents
template <typename DataProvider>
void populateVertexData(Ogre::VertexData* vertexData,
                        const DataProvider& provider) {
  vertexData->vertexCount = provider.size();
  for (auto vertexElement : DataProvider::VertexDataParams::vertexElements) {
    boost::apply_visitor(
        PopulateVertexElementDataVisitor<DataProvider>(vertexData, provider),
        vertexElement);
  }
}
}  // end namespace impl

#endif  //_FRAMEWORK_RENDERING_VERTEXDATAIMPL_H_
