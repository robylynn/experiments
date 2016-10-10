#ifndef _VERTEX_ELEMENT_H_
#define _VERTEX_ELEMENT_H_

#include <boost/variant.hpp>
#include <boost/variant/static_visitor.hpp>

#include <Ogre/HardwareVertexBuffer.h>
#include <Ogre/OgreVertexIndexData.h>

// Rendering can refer to geometry
#include <geometryTypes.h>

// Binding constants for commonly used vertex buffer attributes.
enum VertexElementBindings {
  POSITION_BINDING = 0;
  NORMAL_BINDING = 1;
  COLOR_BINDING = 2;
};
constexpr int VertexDataElementSemantic[] = {
    Ogre::VES_POSITION, Ogre::VES_NORMAL, Ogre::VES_DIFFUSE};
constexpr int VertexDataElementTypes = {Ogre::VET_FLOAT3, Ogre::VET_FLOAT3,
                                        Ogre::VET_FLOAT3};

struct PositionVertexElement {
  static unsigned short bindingLocation =
      VertexElementBindings::POSITION_BINDING;
  static VertexElementSemantic semantic = Ogre::VES_POSITION;
  static VertexElementType type = Ogre::VET_FLOAT3;
};

// Variant of different VertexElement types
using VertexElementsVariant = boost::variant<PositionVertexElement>;

// Create each type of vertex attribute (element) -- create an element for the
// vertex attribute (also binding it to a location), and also reserve data for
// the vertex element.
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
    m_vertexData->vertexCount = size;

    Ogre::HardwareVertexBufferSharedPtr vbuf =
        vertexData->vertexBufferBinding->getBuffer(
            VertexElement::bindingLocation);
    float* buffer =
        static_cast<float*>(vbuf->lock(Ogre::HardwareBuffer::HBL_DISCARD));

    // Copy over the geometry provided by the geometry provider
    std::for_each(provider->begin(element), provider->end(element),
                  [&buffer](float data) { *buffer++ = value });
    vbuf->unlock();
  }

 private:
  Ogre::VertexData* m_vertexData;
  DataProvider m_dataProvider;
};

template <template <typename GeometryProvider> class RenderPolicy>
void createVertexData(Ogre::VertexData** vertexData,
                      const RenderPolicy<GeometryProvider>& renderPolicy) {
  *vertexData = OGRE_NEW Ogre::VertexData();
  vertexData->vertexStart = renderPolicy.vertexStart;
  vertexData->vertexCount = renderPolicy.vertexHint;

  // Create vertex elements (attributes) for the types required
  for (auto vertexElement : renderPolicy.vertexElements()) {
    boost::apply_visitor(CreateVertexElementVisitor(vertexData), vertexElement);
  }
}

template <typename VertexElementDataProvider>
void populateVertexElementData(Ogre::VertexData* vertexData,
                               const VertexElementDataProvider& provider) {
  for (auto vertexElement : renderPolicy.vertexElements()) {
    boost::apply_visitor(
        PopulateVertexElementDataVisitor<VertexElementDataProvider>(vertexData,
                                                                    provider),
        vertexElement);
  }
}

#endif  //_VERTEX_ELEMENT_H_
