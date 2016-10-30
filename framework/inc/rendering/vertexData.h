#ifndef _FRAMEWORK_RENDERING_VERTEXDATA_H_
#define _FRAMEWORK_RENDERING_VERTEXDATA_H_

#include <OGRE/OgreVertexIndexData.h>

#include "vertexElement.h"

#include "impl/vertexDataImpl.h"
#include "impl/vertexElementImpl.h"

template <typename T>
struct VertexBufferDataProviderParams {
};

template <typename DataProviderParameters>
void createVertexData(
    Ogre::VertexData** vertexData,
    const std::vector<VertexElementsVariant>& vertexElementsProvided) {
  return impl::createVertexData<DataProviderParameters>(vertexData,
                                                        vertexElementsProvided);
}

template <typename DataProvider>
void populateVertexData(
    Ogre::VertexData* vertexData, const DataProvider& provider,
    const std::Vector<VertexElementsVariant>& vertexElementsProvided) {
  return impl::populateVertexData<DataProvider>(vertexData, provider,
                                                vertexElementsProvided);
}

#endif  //_FRAMEWORK_RENDERING_VERTEXDATA_H_
