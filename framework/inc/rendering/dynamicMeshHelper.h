#ifndef _FRAMEWORK_RENDERING_DYNAMIC_MESH_HELPER_H_
#define _FRAMEWORK_RENDERING_DYNAMIC_MESH_HELPER_H_

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>

#include "defaultRenderables.h"

// The dynamic mesh helper provides helpers for conveniently creating simple
// meshes and attaching them to a SceneNode. This is useful for a number of
// geometry processing code for visualization, where such entities are created
// to visualize the current state of an algorithm.

// Make a mesh with a uuid name.
template <typename GeometryRep>
std::string make_mesh_renderable(const GeometryRep& geometryRep) {
  static boost::uuids::random_generator uidGenerator;
  return make_mesh_renderable(geometryRep,
                              boost::uuids::to_string(uidGenerator()));
}

#endif  //_FRAMEWORK_RENDERING_DYNAMIC_MESH_HELPER_H_
