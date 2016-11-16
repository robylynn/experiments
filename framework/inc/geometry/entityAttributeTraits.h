#ifndef _FRAMEWORK_GEOMETRY_ENTITY_ATTRIBUTE_TRAITS_H_
#define _FRAMEWORK_GEOMETRY_ENTITY_ATTRIBUTE_TRAITS_H_

// Base attribute traits for various entities. Geometry representations
// specialize the attribute traits for those entities that they furnish.

template <typename GeometryRep>
class VertexAttributeTraits {};

template <typename GeometryRep>
class EdgeAttributeTraits {};

#endif//_FRAMEWORK_GEOMETRY_ENTITY_ATTRIBUTE_TRAITS_H_
