#ifndef _POLYLOOP_GEOMETRY_PROVIDER_H_
#define _POLYLOOP_GEOMETRY_PROVIDER_H_

#include <memory>

#include <CGAL/circulator.h>

#include <OGRE/OgreRenderOperation.h>

#include "polyloop.h"

class PolyloopStripPolicy {
 public:
  static constexpr Ogre::RenderOperation::OperationType PRIMITIVE_TYPE =
      Ogre::RenderOperation::OT_LINE_STRIP;
};

// A geometry provider for the polyloop representation. The polyloop
// representation object itself must remain valid during the use of this
// adaptor class.
template <typename LoopType, typename ProviderPolicy = PolyloopStripPolicy>
class PolyloopGeometryProvider : public ProviderPolicy {
 private:
  const LoopType* m_polyloop;
  using LoopCirculator = CGAL::Circulator_from_iterator<
      typename LoopType::const_iterator>;
  LoopCirculator m_circulator;
  using CirculatorContainer = CGAL::Container_from_circulator<LoopCirculator>;
  CirculatorContainer m_circularContainer;

 public:
  static constexpr int HINT_MAX_BOUND = LoopType::HINT_MAX_BOUND;
  using const_iterator = decltype(std::declval<CirculatorContainer&>().begin());

 public:
  PolyloopGeometryProvider(const LoopType& polyloop);
  PolyloopGeometryProvider(const LoopType&& polyloop) = delete;
  ~PolyloopGeometryProvider() {}

  size_t size() const { return m_polyloop->size() + 1; }

  auto begin() const -> decltype(std::declval<CirculatorContainer&>().begin()) {
    return m_circularContainer.begin();
  }
  auto end() const -> decltype(std::declval<CirculatorContainer&>().end()) {
    return m_circularContainer.end();
  }
};

template <typename LoopType, typename ProviderPolicy>
PolyloopGeometryProvider<LoopType, ProviderPolicy>::PolyloopGeometryProvider(
    const LoopType& polyloop)
    : m_polyloop(&polyloop),
      m_circulator(m_polyloop->begin(), m_polyloop->end()),
      m_circularContainer(m_circulator) {}

#endif  //_POLYLOOP_GEOMETRY_PROVIDER_H_
