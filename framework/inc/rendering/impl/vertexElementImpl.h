#ifndef _FRAMEWORK_RENDERING_VERTEXELEMENTIMPL_H_
#define _FRAMEWORK_RENDERING_VERTEXELEMENTIMPL_H_

#include <boost/variant.hpp>
#include <boost/variant/static_visitor.hpp>

#include <OGRE/OgreHardwareVertexBuffer.h>
#include <OGRE/OgreHardwareBufferManager.h>
#include <OGRE/OgreVertexIndexData.h>

// Rendering can refer to geometry
#include <geometryTypes.h>

// This design is partly inspired by Ogre's rendering design. Here's a
// synopsis:
// A RenderingOperation holds pointers to VertexData, IndexData and a Renderable
// together.
// The VertexData collects vertex-related information used to render the
// geometry. At the heart is a data-store that is a pointer to the vertex data.
// The VertexData object consists of a VertexDeclaration and a
// VertexBufferBinding (a map from the binding location to a pointer to the
// vertex buffer location, and an accessor for accessing the underlying buffer
// associated with the binding). The VertexDeclaration consists of a list of
// VertexElements, with each VertexElement defining the format of the input
// vertex data.
//
// The VertexElement code makes it easier for geometry representations to
// provide data that can be used by Ogre's rendering APIs. Specifically,
// for a given set of commonly occurring VertexElements whose data may be
// provided by a geometry representation (Positions, Normals, etc), here are
// convenience functions that make it possible to the geometry representation to
// provide just commonly provided for types (CGAL::Point, CGAL::Vector, etc).

namespace impl {

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
    // TODO msati3: Allow for HBU_DYNAMIC_WRITE_ONLY to be a parameter
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
//
// The DataProvider must provide begin and end for the element types that are
// contained in the VertexData's bindings.
//
// TODO msati3: Change this interface to accept iterators
template <typename DataProvider>
class PopulateVertexElementDataVisitor : public boost::static_visitor<void> {
 public:
  PopulateVertexElementDataVisitor(Ogre::VertexData* vertexData,
                                   const DataProvider& dataProvider)
      : m_vertexData(vertexData), m_dataProvider(dataProvider) {}

  template <typename VertexElement>
  void operator()(const VertexElement& element) {
    Ogre::HardwareVertexBufferSharedPtr vbuf =
        m_vertexData->vertexBufferBinding->getBuffer(
            VertexElement::bindingLocation);
    float* buffer =
        static_cast<float*>(vbuf->lock(Ogre::HardwareBuffer::HBL_DISCARD));

    // Copy over the geometry provided by the geometry provider
    std::for_each(m_dataProvider.begin(element), m_dataProvider.end(element),
                  [&buffer](float data) {  *buffer++ = data; });
    vbuf->unlock();
  }

 private:
  Ogre::VertexData* m_vertexData;
  DataProvider m_dataProvider;
};

}  // end namespace impl

#endif  //_FRAMEWORK_RENDERING_VERTEXELEMENTIMPL_H_
