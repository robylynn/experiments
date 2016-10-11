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
constexpr int VertexDataElementSemantic[] = {
    Ogre::VES_POSITION, Ogre::VES_NORMAL, Ogre::VES_DIFFUSE};
constexpr int VertexDataElementTypes[] = {Ogre::VET_FLOAT3, Ogre::VET_FLOAT3,
                                          Ogre::VET_FLOAT3};

struct PositionVertexElement {
  constexpr static unsigned short bindingLocation =
      VertexElementBindings::POSITION_BINDING;
  constexpr static Ogre::VertexElementSemantic semantic = Ogre::VES_POSITION;
  constexpr static Ogre::VertexElementType type = Ogre::VET_FLOAT3;
};

// Variant of different VertexElement types
using VertexElementsVariant = boost::variant<PositionVertexElement>;

#endif  //_FRAMEWORK_RENDERING_VERTEXELEMENT_H_
