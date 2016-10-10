#ifndef _SEQUENTIAL_GEOMETRY_MESH_CREATOR_H_
#define _SEQUENTIAL_GEOMETRY_MESH_CREATOR_H_

#include <Ogre/OgreMesh.h>

#include "renderingConstants.h"

constexpr size_t POSITION_BINDING = 0;

// A sequential geometry mesh creator wraps provided geometry in a mesh, thus
// allowing for instancing. It is assumed that the sequential geometry provider
// provides homogeneous geometry, and thus, there are no material changes. The
// mesh creator owns its creator mesh, unless this ownership is requested.
//
// TODO msati3: When moved to OGRE 2.1, this will have to be refactored.
template <typename SequentialGeometryProvider,
          template <typename GeometryProvider> class RenderPolicy =
              DefaultRenderPolicy>
class SequentialGeometryMeshCreator {
 public:
  SequentialGeometryMeshCreator();
  void setRenderData(const SequentialGeometryProvider& geometryProvider);

 private:
  RenderPolicy<SequentialGeometryProvider> m_renderOpProvider;
  Ogre::MeshPtr m_mesh;
  size_t m_maxBufferSize;
};

template <typename SequentialGeometryProvider,
          template <typename GeometryProvider> class RenderPolicy>
SequentialGeometryMeshCreator<SequentialGeometryProvider, RenderPolicy>::
    SequentialGeometryMeshCreator(
        const std::string& meshName,
        const std::string& groupName = DEFAULT_RENDER_GROUP_NAME) {
  m_mesh = Ogre::MeshManager::getSingleton().createManual(meshName, groupName);
  Ogre::SubMesh* submesh = m_mesh->createSubMesh();

  populateVertexData(&m_mesh->sharedVertexData, m_renderOpProvider);

  // Set to infinite bounding box
  // TODO msati3: Perhaps remove this hardcoding later
  Ogre::AxisAlignedBox aabInf;
  aabInf.setInfinite();
  setBoundingBox(aabInf);
}

template <typename SequentialGeometryProvider,
          template <typename GeometryProvider> class RenderPolicy>
void SequentialGeometryRenderable<SequentialGeometryProvider, RenderPolicy>::
    setRenderData(const SequentialGeometryProvider& geometryProvider) {}

#endif  //_SEQUENTIAL_GEOMETRY_RENDERABLE_H_
