#ifndef _FRAMEWORK_RENDERING_VERTEX_ELEMENT_H_
#define _FRAMEWORK_RENDERING_VERTEX_ELEMENT_H_

#include <OGRE/OgreVertexIndexData.h>
#include <OGRE/OgreHardwareVertexBuffer.h>

#include <attributes/attributeTypes.h>

/** A vertex element type allows for embellishing vertex attributes with
 * rendering information.  The rendering framework expects a geometry that is a
 * vertex attribute provider (and is thus tagged with the type of attribute(s))
 * that are being provided. The rendering framework uses this tag to decide how
 * to populate vertex buffers for the rendering engine's consumption.
 */

// Binding constants for commonly used vertex buffer attributes.
enum VertexElementBindings {
  POSITION_BINDING = 0,
  NORMAL_BINDING = 1,
  COLOR_BINDING = 2
};

struct PositionVertexElement {
  constexpr static unsigned short bindingLocation =
      VertexElementBindings::POSITION_BINDING;
  constexpr static Ogre::VertexElementSemantic semantic = Ogre::VES_POSITION;
  constexpr static Ogre::VertexElementType type = Ogre::VET_FLOAT3;
  constexpr static size_t elementSize = 3;
  constexpr static size_t defaultExtension = 0;
  using AtomicType = float;
};

struct ColorVertexElement {
  constexpr static unsigned short bindingLocation =
      VertexElementBindings::COLOR_BINDING;
  constexpr static Ogre::VertexElementSemantic semantic = Ogre::VES_DIFFUSE;
  constexpr static Ogre::VertexElementType type = Ogre::VET_FLOAT4;
  constexpr static size_t elementSize = 4;
  constexpr static size_t defaultExtension = 1;
  using AtomicType = float;
};

/**
 * Binding of vertex element types to vertex attribute types is done by
 * specializing this traits class for a vertex attribute, and providing a type
 * member that refers to the desired VertexElement type.
 */
template <typename VertexAttribute>
struct VertexElementForAttribute {};

template <typename VertexAttribute>
using vertex_element_for_attribute_t =
    typename VertexElementForAttribute<VertexAttribute>::type;

template <>
struct VertexElementForAttribute<PositionAttribute_3> {
  using type = PositionVertexElement;
};

template <>
struct VertexElementForAttribute<ColorAttribute> {
  using type = ColorVertexElement;
};

#endif  //_FRAMEWORK_RENDERING_VERTEX_ELEMENT_H_
