#ifndef _FRAMEWORK_RENDERING_VERTEX_ELEMENT_H_
#define _FRAMEWORK_RENDERING_VERTEX_ELEMENT_H_

#include <boost/variant.hpp>
#include <boost/variant/static_visitor.hpp>

#include <OGRE/OgreHardwareVertexBuffer.h>
#include <OGRE/OgreHardwareBufferManager.h>
#include <OGRE/OgreVertexIndexData.h>

// Rendering can refer to geometry
#include <geometryTypes.h>

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

// Create each type of vertex element (attribute) for a vertex buffer -- create
// an element for the vertex attribute (also binding it to a location), and also
// reserve data for the vertex element.
class CreateVertexElementVisitor : public boost::static_visitor<void> {
 public:
  CreateVertexElementVisitor(Ogre::VertexData* vertexData)
      : m_vertexData(vertexData) {}

  template <typename VertexElement>
  void operator()(const VertexElement& element) {
    Ogre::VertexDeclaration* decl = m_vertexData->vertexDeclaration;
    Ogre::VertexBufferBinding* binding = m_vertexData->vertexBufferBinding;

    // Add vertex element at desired binding location
    decl->addElement(VertexElement::bindingLocation, 0, VertexElement::type,
                     VertexElement::semantic);
    // Reserve space for vertex element binding in vertex buffer
    Ogre::HardwareVertexBufferSharedPtr vbuf =
        Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
            decl->getVertexSize(VertexElement::bindingLocation),
            m_vertexData->vertexCount,
            Ogre::HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY);
    binding->setBinding(VertexElement::bindingLocation, vbuf);
  };

 private:
  Ogre::VertexData* m_vertexData;
};

// Populate data for each vertex element. This requires the client to furnish
// begin and end iterators for providing data. The visitor copies data by
// making a call to the data provider which must provide begin and end
// functions that take in a VertexElement object, and return corresponding
// flattened iterators over floats.
template <typename DataProvider>
class PopulateVertexElementDataVisitor : public boost::static_visitor<void> {
 public:
  PopulateVertexElementDataVisitor(Ogre::VertexData* vertexData,
                                   const DataProvider& dataProvider)
      : m_vertexData(vertexData), m_dataProvider(dataProvider) {}

  template <typename VertexElement>
  void operator()(const VertexElement& element) {
    m_vertexData->vertexCount = m_dataProvider->size();

    Ogre::HardwareVertexBufferSharedPtr vbuf =
        m_vertexData->vertexBufferBinding->getBuffer(
            VertexElement::bindingLocation);
    float* buffer =
        static_cast<float*>(vbuf->lock(Ogre::HardwareBuffer::HBL_DISCARD));

    // Copy over the geometry provided by the geometry provider
    std::for_each(m_dataProvider->begin(element), m_dataProvider->end(element),
                  [&buffer](float data) { *buffer++ = data; });
    vbuf->unlock();
  }

 private:
  Ogre::VertexData* m_vertexData;
  DataProvider m_dataProvider;
};

#endif  //_FRAMEWORK_RENDERING_VERTEX_ELEMENT_H_
