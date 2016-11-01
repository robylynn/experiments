#ifndef _FRAMEWORK_GEOMETRY_DEFAULT_GEOMETRY_PROVIDERS_H_
#define _FRAMEWORK_GEOMETRY_DEFAULT_GEOMETRY_PROVIDERS_H_

#include <CGAL/Polyhedron_3.h>

#include "polyloop_2.h"
#include "polyloop_3.h"
#include "polyloopGeometryProvider.h"
#include "triangleMeshGeometryProvider.h"
#include "vertexElementProviderTraits.h"

template <typename Kernel, typename VertexElement>
struct VertexElementProviderTraits<CGAL::Polyhedron_3<Kernel>, VertexElement> {
 private:
  using MeshType = CGAL::Polyhedron_3<Kernel>;

 public:
  using provider_type = TriangleMeshGeometryProvider<MeshType>;
  using const_iterator =
      typename TriangleMeshGeometryProvider<MeshType>::const_iterator;
  using storage_strategy =
      ElementProviderStorageStrategy<TriangleMeshGeometryProvider<MeshType>>;
};

template <>
struct VertexElementProviderTraits<Polyloop_2, PositionVertexElement> {
  using provider_type = PolyloopGeometryProvider<Polyloop_2>;
  using const_iterator = PolyloopGeometryProvider<Polyloop_2>::const_iterator;
  using storage_strategy =
      ElementProviderStorageStrategy<PolyloopGeometryProvider<Polyloop_2>>;
};

template <>
struct VertexElementProviderTraits<Polyloop_3, PositionVertexElement> {
  using provider_type = PolyloopGeometryProvider<Polyloop_3>;
  using const_iterator = PolyloopGeometryProvider<Polyloop_3>::const_iterator;
  using storage_strategy =
      ElementProviderStorageStrategy<PolyloopGeometryProvider<Polyloop_2>>;
};

#endif  // _FRAMEWORK_GEOMETRY_DEFAULT_GEOMETRY_PROVIDERS_H_
