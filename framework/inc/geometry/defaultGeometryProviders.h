#ifndef _FRAMEWORK_GEOMETRY_DEFAULT_GEOMETRY_PROVIDERS_H_
#define _FRAMEWORK_GEOMETRY_DEFAULT_GEOMETRY_PROVIDERS_H_

#include <CGAL/Polyhedron_3.h>

#include "polyline.h"
#include "polyloop_2.h"
#include "polyloop_3.h"
#include "polyloopGeometryProvider.h"
#include "polylineGeometryProvider.h"
#include "triangleMeshGeometryProvider.h"
#include "vertexElementProviderTraits.h"
#include "defaultElementTypes.h"

template <typename T>
struct VertexElementProviderTraits<std::vector<T>,
                                   typename VertexElementFromType<T>::type> {
  using provider_type = PolylineGeometryProvider<std::vector<T>>;
  using const_iterator =
      typename PolylineGeometryProvider<std::vector<T>>::const_iterator;
  using storage_strategy =
      ElementProviderStorageStrategy<PolylineGeometryProvider<std::vector<T>>>;
};

template <typename PointType>
struct VertexElementProviderTraits<
    Polyline<PointType>, typename VertexElementFromType<PointType>::type> {
  using provider_type = PolylineGeometryProvider<Polyline<PointType>>;
  using const_iterator =
      typename PolylineGeometryProvider<Polyline<PointType>>::const_iterator;
  using storage_strategy = ElementProviderStorageStrategy<
      PolylineGeometryProvider<Polyline<PointType>>>;
};

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
