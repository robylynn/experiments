#ifndef _FRAMEWORK_RENDERING_BUFFER_PROVIDER_TYPES_H_
#define _FRAMEWORK_RENDERING_BUFFER_PROVIDER_TYPES_H_

#include "singleElementProviderAdaptor.h"
#include "tupleElementBufferProvider.h"

// Convenience typedefs for common single element buffer providers obtained
// from Geometry reps.
template <typename GeometryRep,
          typename GeometryProvider = typename VertexElementProviderTraits<
              GeometryRep, PositionVertexElement>::provider_type>
using PositionOnlyBufferProvider = VertexElementBufferProvider<
    SingleElementProviderAdaptor<GeometryProvider, PositionVertexElement>,
    PositionVertexElement>;

template <typename GeometryRep,
          typename ColorProvider = typename VertexElementProviderTraits<
              GeometryRep, ColorVertexElement>::provider_type>
using ColorOnlyBufferProvider = VertexElementBufferProvider<
    SingleElementProviderAdaptor<ColorProvider, ColorVertexElement>,
    ColorVertexElement>;

// Convenience types for some tuple element buffer providers
template <typename DataProvider>
using PositionColorBufferProvider =
    TupleBufferDataProvider<DataProvider, PositionVertexElement,
                            ColorVertexElement>;

#endif //_FRAMEWORK_RENDERING_BUFFER_PROVIDER_TYPES_H_
