#ifndef _FRAMEWORK_RENDERING_DEFAULTRENDERINGPOLICIES_H_
#define _FRAMEWORK_RENDERING_DEFAULTRENDERINGPOLICIES_H_

#include <glog/logging.h>

#include <OGRE/OgreRenderOperation.h>

#include "renderingConstants.h"

// The default RenderPolicy specifies that the operation type is the typedef
// GeometryProvider::PRIMIIVE_TYPE
template <typename VertexBufferDataProvider>
class DefaultRenderPolicy {
 public:
  static constexpr Ogre::RenderOperation::OperationType operationType =
      VertexBufferDataProvider::GeometryProvider::PRIMITIVE_TYPE;
  static constexpr bool useIndexes =
      VertexBufferDataProvider::Params::useIndexes;
};

// The default MaterialPolicy specifies that, depending on the RenderPolicy
// primitive type, the material is such that, for point primitives, black color
// is used, for line primitives, black colored lines with red colored points
// are drawn, while, for triangle primitives, yellow filled triangles, with
// black edges, and red vertices are drawn. These colors are parameterized in
// the materials constructor, though, and can be overridden.
template <typename RenderPolicy>
class DefaultMaterialPolicy {
 public:
  DefaultMaterialPolicy(const RenderPolicy& renderPolicy) {
    switch (renderPolicy.operationType) {
      case Ogre::RenderOperation::OT_POINT_LIST:
        m_material = DEFAULT_POINT_MATERIAL_NAME;
        break;
      case Ogre::RenderOperation::OT_LINE_LIST:
      case Ogre::RenderOperation::OT_LINE_STRIP:
        m_material = DEFAULT_LINE_MATERIAL_NAME;
        break;
      case Ogre::RenderOperation::OT_TRIANGLE_LIST:
      case Ogre::RenderOperation::OT_TRIANGLE_STRIP:
      case Ogre::RenderOperation::OT_TRIANGLE_FAN:
        m_material = DEFAULT_TRIANGLE_MATERIAL_NAME;
        break;
      default:
        LOG(ERROR) << "Received an unhandled primitive type for "
                      "DefaultMaterialPolicy's associated RenderPolicy"
                   << renderPolicy.operationType;
        break;
    }
  }

  std::string getMaterial() { return m_material; }

 private:
  std::string m_material;
};

#endif  // _FRAMEWORK_RENDERING_DEFAULTRENDERINGPOLICIES_H_
