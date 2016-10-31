#ifndef _FRAMEWORK_GEOMETRY_DEFAULT_GEOMETRY_PROVIDERS_H_
#define _FRAMEWORK_GEOMETRY_DEFAULT_GEOMETRY_PROVIDERS_H_

#include "polyloop_2.h"
#include "polyloop_3.h"
#include "polyloopGeometryProvider.h"
#include "vertexElementProviderTraits.h"

template <typename VertexElement>
struct VertexElementProviderTraits<Polyloop_2, VertexElement> {
  using provider_type = PolyloopGeometryProvider<Polyloop_2>;
  using const_iterator = PolyloopGeometryProvider<Polyloop_2>::const_iterator;
  using storage_strategy =
      ElementProviderStorageStrategy<PolyloopGeometryProvider<Polyloop_2>>;
};

template <typename VertexElement>
struct VertexElementProviderTraits<Polyloop_3, VertexElement> {
  using provider_type = PolyloopGeometryProvider<Polyloop_3>;
  using const_iterator = PolyloopGeometryProvider<Polyloop_3>::const_iterator;
  using storage_strategy =
      ElementProviderStorageStrategy<PolyloopGeometryProvider<Polyloop_2>>;
};

#endif  // _FRAMEWORK_GEOMETRY_DEFAULT_GEOMETRY_PROVIDERS_H_
