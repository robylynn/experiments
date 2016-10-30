#ifndef _FRAMEWORK_RENDERING_VERTEXDATA_H_
#define _FRAMEWORK_RENDERING_VERTEXDATA_H_

#include <OGRE/OgreVertexIndexData.h>

#include "vertexElement.h"

#include "impl/vertexDataImpl.h"
#include "impl/vertexElementImpl.h"

template <typename DataProvider>
void createVertexData(Ogre::VertexData** vertexData) {
  return impl::createVertexData<DataProvider>(vertexData);
}

template <typename DataProvider>
void populateVertexData(Ogre::VertexData* vertexData,
                        const DataProvider& provider) {
  return impl::populateVertexData<DataProvider>(vertexData, provider);
}

#endif  //_FRAMEWORK_RENDERING_VERTEXDATA_H_
