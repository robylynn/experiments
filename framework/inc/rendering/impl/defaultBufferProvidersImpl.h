#ifndef _FRAMEWORK_RENDERING_IMPL_DEFAULT_BUFFER_PROVIDERS_IMPL_H_
#define _FRAMEWORK_RENDERING_IMPL_DEFAULT_BUFFER_PROVIDERS_IMPL_H_

namespace impl {

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
} // end of namespace impl

#endif  //_FRAMEWORK_RENDERING_IMPL_DEFAULT_BUFFER_PROVIDERS_IMPL_H_
