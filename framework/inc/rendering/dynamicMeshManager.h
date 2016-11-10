#ifndef _FRAMEWORK_RENDERING_DYNAMIC_MESH_MANAGER_H_
#define _FRAMEWORK_RENDERING_DYNAMIC_MESH_MANAGER_H_

#include "ogreUtils.h"
#include "defaultRenderables.h"

// The dynamic mesh helper provides helpers for conveniently creating a single
// instanced mesh and attaching them to a SceneNode. This is useful for a number
// of geometry processing code for visualization, where such entities are
// created to visualize the current state of an algorithm.
//
// The dynamic mesh manager maintains dynamic meshes (those that have only one
// entity instanced off them). This allows the mesh manager to unload the mesh
// when the entity is destroyed.
class DynamicMeshManager {
 public:
  template <typename GeometryRep>
  Ogre::Entity* addMesh(const GeometryRep& geometryRep,
                        Ogre::SceneNode* sceneNode) {
    std::string meshName = make_mesh_renderable(geometryRep);
    Ogre::Entity* entity = OgreUtils::addMeshInstance(meshName, sceneNode);
    m_entityMeshMap.insert(std::make_pair(entity, meshName));
    return entity;
  }

 private:
  std::map<Ogre::Entity*, std::string> m_entityMeshMap;
};

#endif  //_FRAMEWORK_RENDERING_DYNAMIC_MESH_MANAGER_H_
