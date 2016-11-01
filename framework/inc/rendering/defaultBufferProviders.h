#ifndef _FRAMEWORK_RENDERING_DEFAULT_BUFFER_PROVIDERS_H_
#define _FRAMEOWKR_RENDERING_DEFAULT_BUFFER_PROVIDERS_H_

#include "vertexElementBufferProvider.h"
#include "singleElementProviderAdaptor.h"
#include "vertexElementProviderTraits.h"

#include "bufferProviderTypes.h"
#include "impl/defaultBufferProvidersImpl.h"

template <typename GeometryRep>
PositionOnlyBufferProvider<GeometryRep> make_position_buffer_provider(
    const GeometryRep& geometryRep) {
  using provider_type = typename VertexElementProviderTraits<
      GeometryRep, PositionVertexElement>::provider_type;
  provider_type* dummyPtr = nullptr;
  return impl::make_position_buffer_provider_internal(geometryRep, dummyPtr);
}

template <typename GeometryRep>
ColorOnlyBufferProvider<GeometryRep> make_color_buffer_provider(
    const GeometryRep& geometryRep) {
  return ColorOnlyBufferProvider<GeometryRep>(geometryRep);
}

#endif  //_FRAMEWORK_RENDERING_DEFAULT_BUFFER_PROVIDERS_H_
