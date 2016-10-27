#ifndef _SEQUENTIAL_GEOMETRY_RENDERABLE_H_
#define _SEQUENTIAL_GEOMETRY_RENDERABLE_H_

#include <glog/logging.h>

#include "defaultRenderingPolicies.h"
#include "vertexData.h"

// A sequential geometry renderable allows for the rendering of sequential
// geometry. This class is primarily usable for simple geometries, that will
// not require instancing. If support for instancing is desired, look to the
// SequentialGeometryMesh class
// TODO msati3: When moved to OGRE 2.1, this will have to be refactored.

// In current OGRE design, a SimpleRenderable contains both a Material
// and a RenderOperation, aside from the VertexData itself. This is abstracted
// out of the SequentialGeometryRenderable as two policies
template <typename VertexBufferDataProvider,
          typename RenderPolicy = DefaultRenderPolicy<VertexBufferDataProvider>,
          typename MaterialPolicy = DefaultMaterialPolicy<RenderPolicy>,
          typename BoundingBoxProvider =
              DefaultBoundingBoxProvider<VertexBufferDataProvider>>
class GeometryRenderable : public Ogre::SimpleRenderable {
 public:
  GeometryRenderable(const MaterialPolicy& materialPolicy)
      : m_materialPolicy(materialPolicy) {
    mRenderOp.useIndexes = m_renderPolicy.useIndexes;
    mRenderOp.operationType = m_renderPolicy.operationType;

    // Set material from material policy
    setMaterial(m_materialPolicy());

    LOG(INFO) << "Created a geometry renderable, with renderPolicy type "
              << m_renderPolicy.operationType << " and material "
              << m_materialPolicy();
  }

  GeometryRenderable() : GeometryRenderable(MaterialPolicy(m_renderPolicy)) {}

  void setVertexBufferData(const VertexBufferDataProvider& vertexDataProvider) {
    using Params = typename VertexBufferDataProvider::Params;

    setBoundingBox(BoundingBoxProvider(vertexDataProvider)());

    createVertexData<Params>(&mRenderOp.vertexData);
    populateVertexData<VertexBufferDataProvider>(mRenderOp.vertexData,
                                                 vertexDataProvider);
    LOG(INFO) << "Added a geometry renderable to a scene, with number of "
                 "vertex coordinates " << vertexDataProvider.size()
              << std::endl;
    if (!mRenderOp.useIndexes) {
      mRenderOp.indexData = 0;
    } else {
      // TODO msati3: Index provider code here, that takes in a indexData
      // pointer, and a geometryProvider / indexProvider
    }
  }

  // These are necessary to aid in Ogre Scene Management (spatial subdivision
  // data-structure maintainance).
  // TODO msati3: Fix these values?
  Ogre::Real getSquaredViewDepth(const Ogre::Camera* cam) const override {
    return 100;
  }
  Ogre::Real getBoundingRadius() const override { return 1000; }

 private:
  RenderPolicy m_renderPolicy;
  MaterialPolicy m_materialPolicy;
};

#endif  //_SEQUENTIAL_GEOMETRY_RENDERABLE_H_
