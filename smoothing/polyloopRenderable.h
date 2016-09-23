#ifndef _POLYLOOP_RENDERABLE_H_
#define _POLYLOOP_RENDERABLE_H_

#include <OGRE/Ogre.h>
#include <OGRE/OgreSimpleRenderable.h>

#include "polyloop.h"

// TODO msati3: Create an abstraction for immediate mode renderable objects,
// allowing for faster prototyping. Perhaps, also subclass for objects with
// changing geometry size.

// TODO msati3: Move this to an outside class perhaps?
constexpr size_t POSITION_BINDING = 0;

// A renderable polyloop class
// TODO msati3: When moved to OGRE 2.1, this will have to be refactored.
template <typename T>
class PolyloopRenderable : public Ogre::SimpleRenderable {
 public:
  PolyloopRenderable(const Polyloop<T>& polyloop,
                     const std::string& materialName = "Materials/EdgesAndVertices");
  ~PolyloopRenderable() {}

  // TODO msati3: This can perhaps be done with a generic mechanism to update
  // whenever the geometry is modified?
  void updatePolyloop(const Polyloop<T>& polyloop);

  // These are necessary to aid in Ogre Scene Management (spatial subdivision
  // data-structure maintainance).
  // TODO msati3: Fix these values?
  Ogre::Real getSquaredViewDepth(const Ogre::Camera* cam) const override {
    return 100;
  }
  Ogre::Real getBoundingRadius() const override { return 1000; }
};

template <typename T>
PolyloopRenderable<T>::PolyloopRenderable(const Polyloop<T>& polyloop,
                                          const std::string& materialName) {
  mRenderOp.vertexData = OGRE_NEW Ogre::VertexData();
  mRenderOp.vertexData->vertexStart = 0;
  mRenderOp.vertexData->vertexCount = Polyloop<T>::HINT_MAX_BOUND;
  mRenderOp.useIndexes = false;
  mRenderOp.indexData = 0;
  mRenderOp.operationType = Ogre::RenderOperation::OT_LINE_STRIP;

  // Set desired material. The default material renders lines between edges,
  // and point at vertex locations in a second pass.
  setMaterial(materialName);

  Ogre::VertexDeclaration* decl = mRenderOp.vertexData->vertexDeclaration;
  Ogre::VertexBufferBinding* binding =
      mRenderOp.vertexData->vertexBufferBinding;

  // TODO msati3: Handle generic formats dependant on the current CGAL Kernel
  decl->addElement(POSITION_BINDING, 0, Ogre::VET_FLOAT3, Ogre::VES_POSITION);

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

  updatePolyloop(polyloop);
}

template <typename T>
void PolyloopRenderable<T>::updatePolyloop(const Polyloop<T>& polyloop) {
  // Set the new vertex count (the +1 is because we close the loop)
  mRenderOp.vertexData->vertexCount = polyloop.size() + 1;

  // Copy over the geometry of the polyloop
  Ogre::HardwareVertexBufferSharedPtr vbuf =
      mRenderOp.vertexData->vertexBufferBinding->getBuffer(POSITION_BINDING);
  float* buffer =
      static_cast<float*>(vbuf->lock(Ogre::HardwareBuffer::HBL_DISCARD));

  std::for_each(polyloop.begin(), polyloop.end(), [&buffer](const T& point) {
    *buffer++ = point.x();
    *buffer++ = point.y();
    *buffer++ = point.z();
  });
  // Add the first point again to "complete" the polyloop geometry
  *buffer++ = polyloop.begin()->x();
  *buffer++ = polyloop.begin()->y();
  *buffer++ = polyloop.begin()->z();

  vbuf->unlock();
}

#endif  //_POLYLOOP_RENDERABLE_H_
