#ifndef _FRAMEWORK_RENDERING_VERTEXELEMENT_H_
#define _FRAMEWORK_RENDERING_VERTEXELEMENT_H_

#include <boost/variant.hpp>

#include <OGRE/OgreVertexIndexData.h>
#include <OGRE/OgreHardwareVertexBuffer.h>

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
}

// Variant of different VertexElement types
using VertexElementsVariant =
    boost::variant<PositionVertexElement, ColorVertexElement>;

#endif  //_FRAMEWORK_RENDERING_VERTEXELEMENT_H_
