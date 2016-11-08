#ifndef _POLYLINE_GEOMETRY_PROVIDER_H_
#define _POLYLINE_GEOMETRY_PROVIDER_H_

#include <memory>

#include <OGRE/OgreRenderOperation.h>

#include "containerAlgorithms.h"
#include "vertexElement.h"
#include "vertexElementProviderTraits.h"

class PolylinePointPolicy {
 public:
  static constexpr Ogre::RenderOperation::OperationType PRIMITIVE_TYPE =
      Ogre::RenderOperation::OT_POINT_LIST;
  static constexpr size_t VERTICES_PER_BASE = 1;
  static const std::array<int, VERTICES_PER_BASE>& stencil() {
    static constexpr std::array<int, VERTICES_PER_BASE> stencil = {{0}};
    return stencil;
  }
};

class PolylineListPolicy {
 public:
  static constexpr Ogre::RenderOperation::OperationType PRIMITIVE_TYPE =
      Ogre::RenderOperation::OT_LINE_LIST;
  static constexpr size_t VERTICES_PER_BASE = 2;
  static const std::array<int, VERTICES_PER_BASE>& stencil() {
    static constexpr std::array<int, VERTICES_PER_BASE> stencil = {{0, 1}};
    return stencil;
  }
};

// This policy plays nice when using to supply the vertex buffer data for a
// manual mesh created via the GeometryMeshCreator.
class PolylineMeshPolicy {
 public:
  static constexpr Ogre::RenderOperation::OperationType PRIMITIVE_TYPE =
      Ogre::RenderOperation::OT_TRIANGLE_LIST;
  static constexpr size_t VERTICES_PER_BASE = 3;
  static const std::array<int, VERTICES_PER_BASE>& stencil() {
    static constexpr std::array<int, VERTICES_PER_BASE> stencil = {{0, 1, 0}};
    return stencil;
  }
};

// A geometry provider for the polyline representation. The polyline
// representation object itself must remain valid during the use of this
// adaptor class, and provide an iterator over points of the polyline.
template <typename LineType, typename ProviderPolicy = PolylineListPolicy>
class PolylineGeometryProvider : public ProviderPolicy {
 private:
  using LineIterator = typename LineType::const_iterator;

  const LineType* m_polyline;

  // The LineTriangleAdaptor iterator adapts a LineIterator to return a
  // degenerate triangle by returning 3 values (*iter, *(iter+1), *iter) before
  // a single LineIterator increment
 public:
  static constexpr int HINT_MAX_BOUND = 10000;
  using const_iterator =
      utils::stencil_iterator<LineIterator, ProviderPolicy::VERTICES_PER_BASE>;

  PolylineGeometryProvider(const LineType& polyline) : m_polyline(&polyline) {}
  PolylineGeometryProvider(const LineType&& polyline) = delete;
  ~PolylineGeometryProvider() {}

  size_t size() const {
    return ProviderPolicy::VERTICES_PER_BASE * m_polyline->size();
  }

  const_iterator begin() const {
    return const_iterator(m_polyline->begin(), ProviderPolicy::stencil());
  }

  const_iterator end() const {
    return const_iterator(m_polyline->end() - 1, ProviderPolicy::stencil());
  }
};

// A polyline geometry provider is a lightweight object. So, we specialize the
// storage policy to by value. This allows for nicer client syntax through
// implicit temporary creation for PolylopGeometryProvider.
template <typename ElementProvider, typename ElementType>
class ElementProviderStorageStrategy<
    PolylineGeometryProvider<ElementProvider, ElementType>>
    : public CopyProviderStorageStrategy<
          PolylineGeometryProvider<ElementProvider, ElementType>> {
 protected:
  using CopyProviderStorageStrategy<PolylineGeometryProvider<
      ElementProvider, ElementType>>::CopyProviderStorageStrategy;
};

#endif  //_POLYLINE_GEOMETRY_PROVIDER_H_
