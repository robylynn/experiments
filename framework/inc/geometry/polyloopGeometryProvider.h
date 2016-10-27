#ifndef _POLYLOOP_GEOMETRY_PROVIDER_H_
#define _POLYLOOP_GEOMETRY_PROVIDER_H_

#include <memory>

#include <CGAL/circulator.h>

#include <OGRE/OgreRenderOperation.h>

#include "containerAlgorithms.h"

class PolyloopListPolicy {
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
class PolyloopMeshPolicy {
 public:
  static constexpr Ogre::RenderOperation::OperationType PRIMITIVE_TYPE =
      Ogre::RenderOperation::OT_TRIANGLE_LIST;
  static constexpr size_t VERTICES_PER_BASE = 3;
  static const std::array<int, VERTICES_PER_BASE>& stencil() {
    static constexpr std::array<int, VERTICES_PER_BASE> stencil = {{0, 1, 0}};
    return stencil;
  }
};

// A geometry provider for the polyloop representation. The polyloop
// representation object itself must remain valid during the use of this
// adaptor class, and provide an iterator over points of the loop. The first
// and last points are connected by the geometry provider
template <typename LoopType, typename ProviderPolicy = PolyloopListPolicy>
class PolyloopGeometryProvider : public ProviderPolicy {
 private:
  const LoopType* m_polyloop;
  using LoopCirculator =
      CGAL::Circulator_from_iterator<typename LoopType::const_iterator>;
  LoopCirculator m_circulator;
  using CirculatorContainer = CGAL::Container_from_circulator<LoopCirculator>;
  CirculatorContainer m_circularContainer;
  using LoopIterator = decltype(std::declval<CirculatorContainer&>().begin());

  // The LoopTriangleAdaptor iterator adapts a LoopIterator to return a
  // degenerate triangle by returning 3 values (*iter, *(iter+1), *iter) before
  // a single LoopIterator increment

 public:
  static constexpr int HINT_MAX_BOUND = LoopType::HINT_MAX_BOUND;
  using const_iterator =
      utils::stencil_circulator_iterator<LoopIterator,
                                         ProviderPolicy::VERTICES_PER_BASE>;

  PolyloopGeometryProvider(const LoopType& polyloop)
      : m_polyloop(&polyloop),
        m_circulator(polyloop.begin(), polyloop.end()),
        m_circularContainer(m_circulator) {}
  PolyloopGeometryProvider(const LoopType&& polyloop) = delete;
  ~PolyloopGeometryProvider() {}

  size_t size() const {
    return ProviderPolicy::VERTICES_PER_BASE * m_polyloop->size();
  }

  const_iterator begin() const {
    return const_iterator(m_circularContainer.begin(),
                          ProviderPolicy::stencil());
  }

  const_iterator end() const {
    return const_iterator(m_circularContainer.end(), ProviderPolicy::stencil());
  }
};

#endif  //_POLYLOOP_GEOMETRY_PROVIDER_H_
