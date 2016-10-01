#ifndef _SEQUENTIAL_GEOMETRY_RENDERABLE_H_
#define _SEQUENTIAL_GEOMETRY_RENDERABLE_H_

constexpr size_t POSITION_BINDING = 0;

// A DefaultRenderOpParamProvider class provides defaults render operation
// parameters that are suitable for a shape representation that is a simple
// container of the geometry it represents.
template <typename GeometryProvider>
class DefaultRenderOpParamsProvider {
 public:
  DefaultRenderOpParamsProvider() {
    vertexStart = 0;
    useIndexes = false;
    vertexHint = GeometryProvider::HINT_MAX_BOUND;
    operationType = GeometryProvider::PRIMITIVE_TYPE;
  }

  size_t vertexStart;
  size_t vertexHint;
  bool useIndexes;
  Ogre::RenderOperation::OperationType operationType;
};

// A sequential geometry renderable allows for the rendering of sequential
// geometry.
// TODO msati3: When moved to OGRE 2.1, this will have to be refactored.
template <typename SequentialGeometryProvider,
          template <typename GeometryProvider> class RenderOpParamsProvider =
              DefaultRenderOpParamsProvider>
class SequentialGeometryRenderable : public Ogre::SimpleRenderable {
 public:
  SequentialGeometryRenderable();
  void setRenderData(const SequentialGeometryProvider& geometryProvider);

  // These are necessary to aid in Ogre Scene Management (spatial subdivision
  // data-structure maintainance).
  // TODO msati3: Fix these values?
  Ogre::Real getSquaredViewDepth(const Ogre::Camera* cam) const override {
    return 100;
  }
  Ogre::Real getBoundingRadius() const override { return 1000; }

 private:
  RenderOpParamsProvider<SequentialGeometryProvider> m_renderOpProvider;
  size_t m_maxBufferSize;
};

template <typename SequentialGeometryProvider,
          template <typename GeometryProvider> class RenderOpParamsProvider>
SequentialGeometryRenderable<
    SequentialGeometryProvider,
    RenderOpParamsProvider>::SequentialGeometryRenderable() {
  mRenderOp.vertexData = OGRE_NEW Ogre::VertexData();
  mRenderOp.vertexData->vertexStart = m_renderOpProvider.vertexStart;

  // Initial vertex count data is used to reserve memory on the GPU.
  mRenderOp.vertexData->vertexCount = m_renderOpProvider.vertexHint;
  m_maxBufferSize = m_renderOpProvider.vertexHint;

  mRenderOp.useIndexes = m_renderOpProvider.useIndexes;
  if (!mRenderOp.useIndexes) {
    mRenderOp.indexData = 0;
  }
  mRenderOp.operationType = m_renderOpProvider.operationType;

  Ogre::VertexDeclaration* decl = mRenderOp.vertexData->vertexDeclaration;
  Ogre::VertexBufferBinding* binding =
      mRenderOp.vertexData->vertexBufferBinding;

  decl->addElement(POSITION_BINDING, 0, Ogre::VET_FLOAT3, Ogre::VES_POSITION);

  // TODO msati3: abstract out policy for vertex buffer handling
  Ogre::HardwareVertexBufferSharedPtr vbuf =
      Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
          decl->getVertexSize(POSITION_BINDING),
          mRenderOp.vertexData->vertexCount,
          Ogre::HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY);
  binding->setBinding(POSITION_BINDING, vbuf);

  // Set to infinite bounding box
  // TODO msati3: Perhaps remove this hardcoding later
  Ogre::AxisAlignedBox aabInf;
  aabInf.setInfinite();
  setBoundingBox(aabInf);
}

template <typename SequentialGeometryProvider,
          template <typename GeometryProvider> class RenderOpParamsProvider>
void SequentialGeometryRenderable<SequentialGeometryProvider,
                                  RenderOpParamsProvider>::
    setRenderData(const SequentialGeometryProvider& geometryProvider) {
  mRenderOp.vertexData->vertexCount = geometryProvider.size();
  //setMaterial(geometryProvider.operationType);

  Ogre::HardwareVertexBufferSharedPtr vbuf =
      mRenderOp.vertexData->vertexBufferBinding->getBuffer(POSITION_BINDING);
  float* buffer =
      static_cast<float*>(vbuf->lock(Ogre::HardwareBuffer::HBL_DISCARD));

  // Copy over the geometry provided by the geometry provider.
  std::for_each(geometryProvider.begin(), geometryProvider.end(),
                [&buffer](const CGAL::Cartesian<float>::Point_3 point) {
                  *buffer++ = point.x();
                  *buffer++ = point.y();
                  *buffer++ = point.z();
                });

  vbuf->unlock();
}

#endif  //_SEQUENTIAL_GEOMETRY_RENDERABLE_H_
