#ifndef _SEQUENTIAL_GEOMETRY_RENDERABLE_H_
#define _SEQUENTIAL_GEOMETRY_RENDERABLE_H_

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
          template <typename GP> class RenderPolicy = DefaultRenderPolicy<
              typename VertexBufferDataProvider::GeometryProvider>,
          template <typename RP> class MaterialPolicy =
              DefaultMaterialPolicy<RenderPolicy>>
class GeometryRenderable : public Ogre::SimpleRenderable {
 public:
  GeometryRenderable() {
    mRenderOp.useIndexes = m_renderPolicy.useIndexes;
    mRenderOp.operationType = m_renderPolicy.operationType;

    // Set material from material policy
    setMaterial(m_materialPolicy.getMaterial());

    // Set to infinite bounding box
    // TODO msati3: Perhaps remove this hardcoding later
    Ogre::AxisAlignedBox aabInf;
    aabInf.setInfinite();
    setBoundingBox(aabInf);
  }

  void setVertexData(const VertexBufferDataProvider& geometryProvider) {
    createVertexData<VertexBufferDataProvider::Params>(
        &m_mesh->sharedVertexData);
    populateVertexData<VertexBufferDataProvider>(m_mesh->sharedVertexData,
                                                 geometryProvider);
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
  RenderPolicy<VertexBufferDataProvider> m_renderPolicy;
  MaterialPolicy<RenderPolicy<VertexBufferDataProvider>> m_materialPolicy;
};

#endif  //_SEQUENTIAL_GEOMETRY_RENDERABLE_H_
