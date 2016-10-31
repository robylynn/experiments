#ifndef _FRAMEWORK_RENDERING_DEFAULT_BUFFER_PROVIDERS_H_
#define _FRAMEOWKR_RENDERING_DEFAULT_BUFFER_PROVIDERS_H_

#include "vertexElementBufferProvider.h"
#include "singleElementBufferProvider.h"
#include "vertexElementProviderTraits.h"

// Convenience typedefs for common single element buffer providers obtained
// from Geometry reps.
template <typename GeometryRep,
          typename GeometryProvider = typename VertexElementProviderTraits<
              GeometryRep, PositionVertexElement>::provider_type>
using PositionOnlyBufferProvider = VertexElementBufferProvider<
    SingleElementBufferProviderAdaptor<GeometryProvider, PositionVertexElement>,
    PositionVertexElement>;

template <typename GeometryRep,
          typename ColorProvider = typename VertexElementProviderTraits<
              GeometryRep, ColorVertexElement>::provider_type>
using ColorOnlyBufferProvider = VertexElementBufferProvider<
    SingleElementBufferProviderAdaptor<ColorProvider, ColorVertexElement>,
    ColorVertexElement>;

template <typename GeometryRep, typename GeometryProvider>
PositionOnlyBufferProvider<GeometryRep> make_position_buffer_provider_internal(
    const GeometryRep& geometryRep, const GeometryProvider* /**/) {
  return PositionOnlyBufferProvider<GeometryRep>(
      typename VertexElementProviderTraits<
          GeometryRep, PositionVertexElement>::provider_type(geometryRep));
}

template <typename GeometryRep>
PositionOnlyBufferProvider<GeometryRep> make_position_buffer_provider_internal(
    const GeometryRep& geometryRep, const GeometryRep* /**/) {
  return PositionOnlyBufferProvider<GeometryRep>(geometryRep);
}

template <typename GeometryRep>
PositionOnlyBufferProvider<GeometryRep> make_position_buffer_provider(
    const GeometryRep& geometryRep) {
  using provider_type = typename VertexElementProviderTraits<
      GeometryRep, PositionVertexElement>::provider_type;
  provider_type* dummyPtr = nullptr;
  return make_position_buffer_provider_internal(geometryRep, dummyPtr);
}

template <typename GeometryRep>
ColorOnlyBufferProvider<GeometryRep> make_color_buffer_provider(
    const GeometryRep& geometryRep) {
  return ColorOnlyBufferProvider<GeometryRep>(geometryRep);
}

#endif  //_FRAMEWORK_RENDERING_DEFAULT_BUFFER_PROVIDERS_H_
