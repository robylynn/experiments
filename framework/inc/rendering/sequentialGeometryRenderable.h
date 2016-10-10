#ifndef _SEQUENTIAL_GEOMETRY_RENDERABLE_H_
#define _SEQUENTIAL_GEOMETRY_RENDERABLE_H_

// A DefaultRenderPolicy class provides defaults render operation
// parameters that are suitable for a shape representation that is a simple
// container of the geometry it represents, and contains just position bindings
template <typename GeometryProvider>
class DefaultRenderPolicy {
 public:
  DefaultRenderPolicy() : m_vertexElements(PositionVertexElement()) {
    vertexStart = 0;
    useIndexes = false;
    vertexHint = GeometryProvider::HINT_MAX_BOUND;
    operationType = GeometryProvider::PRIMITIVE_TYPE;
  }

  void initPolicyParams(const GeometryProvider& provider) {
    vertexHint = provider.sizeHint();
  }

  auto vertexElements() -> decltype(m_vertexElements) {
    return m_vertexElements;
  }

  size_t vertexStart;
  size_t vertexHint;
  bool useIndexes;
  Ogre::RenderOperation::OperationType operationType;

  // TODO msati3: This could be done with type-lists only. Then, one can take
  // the typelist as a parameter, and remove the constraint that
  // DefaultRenderPolicy can only provide for position bindings.
  std::vector<VertexElementsVariant> m_vertexElements;
};

// A sequential geometry renderable allows for the rendering of sequential
// geometry. This class is primarily usable for simple geometries, that will
// not require instancing. If support for instancing is desired, look to the
// SequentialGeometryMesh class
// TODO msati3: When moved to OGRE 2.1, this will have to be refactored.
template <typename SequentialGeometryProvider,
          template <typename GeometryProvider> class RenderPolicy =
              DefaultRenderPolicy>
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
  RenderPolicy<SequentialGeometryProvider> m_renderOpProvider;
  size_t m_maxBufferSize;
};

template <typename SequentialGeometryProvider,
          template <typename GeometryProvider> class RenderPolicy>
SequentialGeometryRenderable<SequentialGeometryProvider,
                             RenderPolicy>::SequentialGeometryRenderable() {
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
          template <typename GeometryProvider> class RenderPolicy>
void SequentialGeometryRenderable<SequentialGeometryProvider, RenderPolicy>::
    setRenderData(const SequentialGeometryProvider& geometryProvider) {
  mRenderOp.vertexData->vertexCount = geometryProvider.size();
  // setMaterial(geometryProvider.operationType);

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
