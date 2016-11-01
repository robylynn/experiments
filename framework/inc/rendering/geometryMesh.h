#ifndef _GEOMETRY_MESH_CREATOR_H_
#define _GEOMETRY_MESH_CREATOR_H_

#include <glog/logging.h>

#include <OGRE/OgreMesh.h>

#include "renderingConstants.h"
#include "defaultRenderingPolicies.h"
#include "vertexData.h"
#include "vertexElement.h"

// A sequential geometry mesh creator wraps provided geometry in a mesh, thus
// allowing for instancing. It is assumed that the sequential geometry provider
// provides homogeneous geometry, and thus, there are no material changes. The
// mesh creator owns its creator mesh, unless this ownership is requested.
//
// TODO msati3: When moved to OGRE 2.1, this will have to be refactored.
// TODO msati3: The logic for dynamically resizing has to be added?
template <typename VertexBufferProvider,
          typename RenderPolicy = DefaultRenderPolicy<VertexBufferProvider>,
          typename MaterialPolicy = DefaultMaterialPolicy<RenderPolicy>,
          typename BoundingBoxProvider =
              DefaultBoundingBoxProvider<VertexBufferProvider>>
class GeometryMeshCreator {
 private:
  using FirstBufferElement = typename std::tuple_element<
      0, typename VertexBufferProviderTraits<
             VertexBufferProvider>::vertex_elements>::type;

 public:
  GeometryMeshCreator(const std::string& meshName,
                      const std::string& groupName = DEFAULT_RENDER_GROUP_NAME)
      : m_materialPolicy(m_renderPolicy) {
    static_assert(
        std::is_same<FirstBufferElement, PositionVertexElement>::value,
        "VertexBufferProvider must provide for PositionVertexElement in first "
        "slot");

    m_mesh =
        Ogre::MeshManager::getSingleton().createManual(meshName, groupName);
    Ogre::SubMesh* submesh = m_mesh->createSubMesh();
    submesh->setMaterialName(m_materialPolicy());
    submesh->operationType = m_renderPolicy.operationType;

    LOG(INFO) << "Created a geometry mesh, with renderPolicy type "
              << m_renderPolicy.operationType << " and material "
              << m_materialPolicy();
  }

  void setVertexBufferData(const VertexBufferProvider& vertexDataProvider) {
    BoundingBoxProvider boundingBoxProvider(vertexDataProvider);
    m_mesh->_setBounds(boundingBoxProvider());
    createVertexData<VertexBufferProvider>(&m_mesh->sharedVertexData);

    // TODO msati3: Fix this hardcoding
    m_mesh->_setBoundingSphereRadius(1000);
    populateVertexData(m_mesh->sharedVertexData, vertexDataProvider);

    // Notify mesh loading completion
    m_mesh->load();
  }

 private:
  Ogre::MeshPtr m_mesh;
  RenderPolicy m_renderPolicy;
  MaterialPolicy m_materialPolicy;
};

#endif  //_GEOMETRY_MESH_CREATOR_H_
