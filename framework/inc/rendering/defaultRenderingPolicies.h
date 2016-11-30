#ifndef _FRAMEWORK_RENDERING_DEFAULT_RENDERING_POLICIES_H_
#define _FRAMEWORK_RENDERING_DEFAULT_RENDERING_POLICIES_H_

#include <glog/logging.h>
#include <tuple>

#include <CGAL/bounding_box.h>

#include <OGRE/OgreRenderOperation.h>

#include "containerAlgorithms.h"
#include "geometryInterop.h"
#include "renderingConstants.h"
#include "vertexBufferProviderTraits.h"

// The default RenderPolicy specifies that the operation type is the typedef
// GeometryProvider::PRIMIIVE_TYPE
template <typename VertexBufferProvider>
class DefaultRenderPolicy {
 private:
  using PositionElementProvider = typename std::tuple_element<
      0, typename VertexBufferProviderTraits<
             VertexBufferProvider>::element_providers>::type;

 public:
  static constexpr Ogre::RenderOperation::OperationType operationType =
      VertexElementProviderTraits<
          PositionElementProvider,
          PositionVertexElement>::provided_type::PRIMITIVE_TYPE;
  static constexpr bool useIndexes =
      VertexBufferProviderTraits<VertexBufferProvider>::useIndexes;
};

// The default MaterialPolicy is a unary functor that, depending on the
// RenderPolicy primitive type provides a default material.
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

  std::string operator()() { return m_material; }

 private:
  std::string m_material;
};

// Computes a bounding box off the VertexBufferDataProvider, or, returns a
// default constructed infinite bounding box
template <typename VertexBufferProvider>
class DefaultBoundingBoxProvider {
 public:
  DefaultBoundingBoxProvider() { m_boundingBox.setInfinite(); }

  DefaultBoundingBoxProvider(const VertexBufferProvider& dataProvider) {
    auto pointIterBegin = utils::tuple_iterator<
        decltype(dataProvider.begin()), 3,
        Kernel::Point_3>::begin(dataProvider.begin(PositionVertexElement()),
                                dataProvider.end(PositionVertexElement()));
    // Assume an infinite bounding box for a single point.
    // TODO Critical msati3: Better handling of this?
    if (dataProvider.size() != 6) {
      Kernel::Iso_cuboid_3 cuboid = CGAL::bounding_box(
          pointIterBegin, utils::make_end_tuple_iterator(pointIterBegin));

      m_boundingBox = GeometryInterop::renderingFromGeom(cuboid);
    }
  }

  Ogre::AxisAlignedBox operator()() { return m_boundingBox; }

 private:
  Ogre::AxisAlignedBox m_boundingBox;
};

#endif  // _FRAMEWORK_RENDERING_DEFAULT_RENDERING_POLICIES_H_
