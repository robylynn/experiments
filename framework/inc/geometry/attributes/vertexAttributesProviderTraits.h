#ifndef _FRAMEWORK_GEOMETRY_VERTEX_ATTRIBUTES_PROVIDER_TRAITS_H_
#define _FRAMEWORK_GEOMETRY_VERTEX_ATTRIBUTES_PROVIDER_TRAITS_H_

#include "attributesProviderTraits.h"

/**
 * A VertexAttributesProvider provide's iteration over vertex properties. A
 * VertexAttributes provider is a simplex provider as well. By default we
 * assume that a vertex attribute provider provides a sequence of points that
 * are to be interpreted simply as such. But, a particular vertex attribute
 * provider can specialize for the type of simplex it provides. For example, a
 * PolyloopSimplicialAdaptor instantiated with a LineList policy knows that it
 * is providing an iterator over vertex attributes that are ordered in a
 * LineList format, and specializes the DefaultSimplex type.
 */
template <typename VertexAttributeProvider>
struct DefaultSimplexType {
  using type = PointList;
};

/**
 * A vertex attribute provider provides vertex attributes. This allows for it
 * to be plugged as a provider into the rendering system. A vertex attribute
 * provider also must provide a tag of the simplicial decomposition of the
 * geometry rep resource it is providing. In the default case, this is a point
 * list, but, can this can be specialized by each resource provider.
 */
template <typename ResourceProvider,
          typename SimplexType =
              typename DefaultSimplexType<ResourceProvider>::type>
struct VertexAttributesProviderTraits
    : public ConstAttributesProviderTraits<ResourceProvider>,
      public SimplicialDecompositionProviderTraits<ResourceProvider,
                                                   SimplexType> {};

#endif  // _FRAMEWORK_GEOMETRY_VERTEX_ATTRIBUTES_PROVIDER_TRAITS_H_
