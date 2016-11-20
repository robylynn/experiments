#ifndef _FRAMEWORK_RENDERING_DEFAULT_RENDERABLES_H_
#define _FRAMEWORK_RENDERING_DEFAULT_RENDERABLES_H_

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>

#include "vertexElementBufferProvider.h"
#include "singleElementProviderAdaptor.h"
#include "geometryMesh.h"
#include "geometryRenderable.h"

// By default assume that the GeometryRenderable provides only position vertex
// elements.
template <typename GeometryRep>
GeometryRenderable<VertexElementBufferProvider<
    SingleElementProviderAdaptor<
        typename VertexElementProviderTraits<
            GeometryRep, PositionVertexElement>::provider_type,
        PositionVertexElement>,
    PositionVertexElement>>
make_simple_renderable(const GeometryRep& geometryRep) {
  using GeomProvider = typename VertexElementProviderTraits<
      GeometryRep, PositionVertexElement>::provider_type;
  using PositionProvider =
      SingleElementProviderAdaptor<GeomProvider, PositionVertexElement>;
  using VertexBufferProvider =
      VertexElementBufferProvider<PositionProvider, PositionVertexElement>;
  GeomProvider geomProvider(geometryRep);
  PositionProvider positionProvider(geomProvider);
  VertexBufferProvider bufferProvider(positionProvider);
  GeometryRenderable<decltype(bufferProvider)> renderable;
  renderable.setVertexBufferData(bufferProvider);
  return renderable;
}

// By default assume that Geometry mesh provides only position vertex elements.
template <typename GeometryRep>
std::string make_mesh_renderable(const GeometryRep& geometryRep,
                                 const std::string& meshName) {
  using GeomProvider = typename VertexElementProviderTraits<
      GeometryRep, PositionVertexElement>::provider_type;
  using PositionProvider =
      SingleElementProviderAdaptor<GeomProvider, PositionVertexElement>;
  using VertexBufferProvider =
      VertexElementBufferProvider<PositionProvider, PositionVertexElement>;
  GeomProvider geomProvider(geometryRep);
  PositionProvider positionProvider(geomProvider);
  VertexBufferProvider bufferProvider(positionProvider);
  GeometryMeshCreator<decltype(bufferProvider)> renderable(meshName);
  renderable.setVertexBufferData(bufferProvider);
  return meshName;
}

// Make a mesh with a uuid name.
template <typename GeometryRep>
std::string make_mesh_renderable(const GeometryRep& geometryRep) {
  static boost::uuids::random_generator uidGenerator;
  return make_mesh_renderable(geometryRep,
                              boost::uuids::to_string(uidGenerator()));
}

#endif  //_FRAMEWORK_RENDERING_DEFAULT_RENDERABLES_H_
