#ifndef _FRAMEWORK_GEOMETRY_EDGE_ATTRIBUTES_PROVIDER_TRAITS_H_
#define _FRAMEWORK_GEOMETRY_EDGE_ATTRIBUTES_PROVIDER_TRAITS_H_

#include "attributesProviderTraits.h"

/**
 * An edge attributes provider provides edge attributes.
 */
template <typename ResourceProvider>
struct EdgeAttributesProviderTraits
    : public ConstAttributesProviderTraits<ResourceProvider> {};

#endif  // _FRAMEWORK_GEOMETRY_EDGE_ATTRIBUTES_PROVIDER_TRAITS_H_
