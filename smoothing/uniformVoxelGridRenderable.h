#ifndef _UNIFORM_VOXEL_GRID_RENDERABLE_H_
#define _UNIFORM_VOXEL_GRID_RENDERABLE_H_

#include "geometryType.h"

#include <OGRE/Ogre.h>
#include <OGRE/OgreSimpleRenderable.h>

#include "uniformVoxelGrid.h"

class UniformVoxelGridRenderable : public Ogre::SimpleRenderable {
 public:
  UniformVoxelGridRenderable(const UniformVoxelGrid& voxelGrid);
};

void UniformVoxelGridRenderable::UniformVoxelGridRenderable(
    const UniformVoxelGrid& voxelGrid) {
}
