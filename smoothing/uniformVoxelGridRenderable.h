#ifndef _UNIFORM_VOXEL_GRID_RENDERABLE_H_
#define _UNIFORM_VOXEL_GRID_RENDERABLE_H_

#include "geometryTypes.h"

#include <OGRE/Ogre.h>
#include <OGRE/OgreSimpleRenderable.h>

#include "uniformVoxelGrid.h"

class UniformVoxelGridRenderable : public Ogre::SimpleRenderable {
 public:
  UniformVoxelGridRenderable(
      const UniformVoxelGrid& voxelGrid,
      const std::string& materialName = "Materials/Vertices");
  ~UniformVoxelGridRenderable() {}

  // TODO msati3: Fix these values?
  Ogre::Real getSquaredViewDepth(const Ogre::Camera* cam) const override {
    return 100;
  }

  Ogre::Real getBoundingRadius() const override { return 1000; }

  private:
  void setGeometry(const UniformVoxelGrid& voxelGrid);
};

UniformVoxelGridRenderable::UniformVoxelGridRenderable(
    const UniformVoxelGrid& voxelGrid, const std::string& materialName) {
  mRenderOp.vertexData = OGRE_NEW Ogre::VertexData();
  mRenderOp.vertexData->vertexStart = 0;
  mRenderOp.vertexData->vertexCount = voxelGrid.size();
  mRenderOp.useIndexes = false;
  mRenderOp.indexData = 0;
  mRenderOp.operationType = Ogre::RenderOperation::OT_POINT_LIST;

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

  setGeometry(voxelGrid);
}

void UniformVoxelGridRenderable::setGeometry(const UniformVoxelGrid& voxelGrid) {
  // Copy over the geometry of the voxel grid
  Ogre::HardwareVertexBufferSharedPtr vbuf =
      mRenderOp.vertexData->vertexBufferBinding->getBuffer(POSITION_BINDING);
  float* buffer =
      static_cast<float*>(vbuf->lock(Ogre::HardwareBuffer::HBL_DISCARD));

  std::for_each(voxelGrid.begin(), voxelGrid.end(),
                [&buffer](const UniformVoxelGrid::value_type& point) {
                  *buffer++ = point.x();
                  *buffer++ = point.y();
                  *buffer++ = point.z();
                });

  vbuf->unlock();
}

#endif  //_UNIFORM_VOXEL_GRID_RENDERABLE_H_
