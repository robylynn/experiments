#ifndef _GEOMETRY_MESH_CREATOR_H_
#define _GEOMETRY_MESH_CREATOR_H_

#include <Ogre/OgreMesh.h>

#include "renderingConstants.h"

#include "vertexData.h"

// A sequential geometry mesh creator wraps provided geometry in a mesh, thus
// allowing for instancing. It is assumed that the sequential geometry provider
// provides homogeneous geometry, and thus, there are no material changes. The
// mesh creator owns its creator mesh, unless this ownership is requested.
//
// TODO msati3: When moved to OGRE 2.1, this will have to be refactored.
// TODO msati3: The logic for dynamically resizing has to be added?
template <typename VertexBufferDataProvider>
class GeometryMeshCreator {
 public:
  GeometryMeshCreator(
      const std::string& meshName,
      const std::string& groupName = DEFAULT_RENDER_GROUP_NAME) {
    m_mesh =
        Ogre::MeshManager::getSingleton().createManual(meshName, groupName);
    Ogre::SubMesh* submesh = m_mesh->createSubMesh();

    // Set to infinite bounding box
    // TODO msati3: Perhaps remove this hardcoding later
    Ogre::AxisAlignedBox aabInf;
    aabInf.setInfinite();
    setBoundingBox(aabInf);
  }

  void setVertexBufferData(const VertexBufferDataProvider& vertexDataProvider) {
    createVertexData<VertexBufferDataProvider::Params>(
        &m_mesh->sharedVertexData);
    populateVertexData<VertexBufferDataProvider>(m_mesh->sharedVertexData,
                                                 geometryProvider);
  }

 private:
  RenderPolicy<typename VertexBufferDataProvider::GeometryProvider>
      m_renderOpProvider;
  Ogre::MeshPtr m_mesh;
  size_t m_maxBufferSize;
};

#endif  //_GEOMETRY_RENDERABLE_H_
