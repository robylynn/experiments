#ifndef _FRAMEWORK_RENDERING_GEOMETRY_RENDERABLE_H_
#define _FRAMEWORK_RENDERING_GEOMETRY_RENDERABLE_H_

#include <glog/logging.h>

#include <OGRE/OgreSimpleRenderable.h>

#include "defaultRenderingPolicies.h"
#include "vertexElement.h"
#include "vertexBufferProviderTraits.h"

// A sequential geometry renderable allows for the rendering of sequential
// geometry. This class is primarily usable for simple geometries, that will
// not require instancing. If support for instancing is desired, look to the
// SequentialGeometryMesh class
// TODO msati3: When moved to OGRE 2.1, this will have to be refactored.

// In current OGRE design, a SimpleRenderable contains both a Material
// and a RenderOperation, aside from the VertexData itself. This is abstracted
// out of the SequentialGeometryRenderable as two policies
template <typename VertexBufferProvider,
          typename RenderPolicy = DefaultRenderPolicy<VertexBufferProvider>,
          typename MaterialPolicy = DefaultMaterialPolicy<RenderPolicy>,
          typename BoundingBoxProvider =
              DefaultBoundingBoxProvider<VertexBufferProvider>>
class GeometryRenderable : public Ogre::SimpleRenderable {
 private:
  using FirstBufferElement = typename std::tuple_element<
      0, typename VertexBufferProviderTraits<
             VertexBufferProvider>::vertex_elements>::type;

 public:
  GeometryRenderable() : GeometryRenderable(MaterialPolicy(m_renderPolicy)) {}

  GeometryRenderable(const MaterialPolicy& materialPolicy)
      : m_materialPolicy(materialPolicy) {
    static_assert(
        std::is_same<FirstBufferElement, PositionVertexElement>::value,
        "VertexBufferProvider must provide for PositionVertexElement in first "
        "slot");

    mRenderOp.useIndexes = m_renderPolicy.useIndexes;
    mRenderOp.operationType = m_renderPolicy.operationType;

    // Set material from material policy
    setMaterial(m_materialPolicy());

    LOG(INFO) << "Created a geometry renderable, with renderPolicy type "
              << m_renderPolicy.operationType << " and material "
              << m_materialPolicy();
  }

  void setVertexBufferData(const VertexBufferProvider& vertexDataProvider) {
    BoundingBoxProvider boundingBoxProvider(vertexDataProvider);
    Ogre::AxisAlignedBox boundingBox = boundingBoxProvider();
    float bounds = std::max(boundingBox.getSize().x, boundingBox.getSize().y);
    bounds = std::max(bounds, boundingBox.getSize().z);
    m_boundingRadius = bounds / 2;
    setBoundingBox(boundingBox);

    createVertexData<VertexBufferProvider>(&mRenderOp.vertexData);
    populateVertexData<VertexBufferProvider>(mRenderOp.vertexData,
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
  Ogre::Real m_boundingRadius;
};

#endif  //_FRAMEWORK_RENDERING_GEOMETRY_RENDERABLE_H_
