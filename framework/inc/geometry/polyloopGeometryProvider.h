#ifndef _POLYLOOP_GEOMETRY_PROVIDER_H_
#define _POLYLOOP_GEOMETRY_PROVIDER_H_

#include <memory>

#include <CGAL/circulator.h>

#include <OGRE/OgreRenderOperation.h>

#include "containerAlgorithms.h"
#include "vertexElement.h"
#include "vertexElementProviderTraits.h"

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
  using LoopCirculator =
      CGAL::Circulator_from_iterator<typename LoopType::const_iterator>;
  using CirculatorContainer = CGAL::Container_from_circulator<LoopCirculator>;
  using LoopIterator = decltype(std::declval<CirculatorContainer&>().begin());

  const LoopType* m_polyloop;
  LoopCirculator m_circulator;
  CirculatorContainer m_circularContainer;

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
        m_circulator(m_polyloop->begin(), m_polyloop->end()),
        m_circularContainer(m_circulator) {}
  PolyloopGeometryProvider(const LoopType&& polyloop) = delete;
  ~PolyloopGeometryProvider() {}

  PolyloopGeometryProvider(const PolyloopGeometryProvider& provider) {
   m_polyloop = provider.m_polyloop;
   m_circulator = LoopCirculator(m_polyloop->begin(), m_polyloop->end());
   m_circularContainer = CirculatorContainer(m_circulator);
  }

  PolyloopGeometryProvider(PolyloopGeometryProvider&& provider)
      : m_polyloop(provider.m_polyloop),
        m_circulator(m_polyloop->begin(), m_polyloop->end()),
        m_circularContainer(m_circulator) {}

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

// A polyloop geometry provider is a lightweight object. So, we specialize the
// storage policy to by value. This allows for nicer client syntax through
// implicit temporary creation for PolylopGeometryProvider.
template <typename ElementProvider, typename ElementType>
class ElementProviderStorageStrategy<
    PolyloopGeometryProvider<ElementProvider, ElementType>>
    : public CopyProviderStorageStrategy<
          PolyloopGeometryProvider<ElementProvider, ElementType>> {
 protected:
  using CopyProviderStorageStrategy<PolyloopGeometryProvider<
      ElementProvider, ElementType>>::CopyProviderStorageStrategy;
};

#endif  //_POLYLOOP_GEOMETRY_PROVIDER_H_
