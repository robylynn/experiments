#ifndef _FRAMEWORK_RENDERING_VERTEXDATA_H_
#define _FRAMEWORK_RENDERING_VERTEXDATA_H_

#include <OGRE/OgreVertexIndexData.h>

#include "vertexElement.h"

#include "impl/vertexDataImpl.h"
#include "impl/vertexElementImpl.h"

template <typename T>
class VertexBufferDataProviderParams {
 public:
  static const size_t vertexStart = 0;
  static const size_t maxBound = 0;
  static const bool useIndexes = false;

  // TODO msati3: This could be done with type-lists only. Then, one can take
  // the typelist as a parameter, and remove the constraint that
  // DefaultRenderPolicy can only provide for position bindings.
  static std::vector<VertexElementsVariant> vertexElements;
};

template <typename DataProviderParameters>
void createVertexData(Ogre::VertexData** vertexData) {
  return impl::createVertexData<DataProviderParameters>(vertexData);
}

template <typename DataProvider>
void populateVertexData(Ogre::VertexData* vertexData,
                        const DataProvider& provider) {
  return impl::populateVertexData<DataProvider>(vertexData, provider);
}

#endif  //_FRAMEWORK_RENDERING_VERTEXDATA_H_
