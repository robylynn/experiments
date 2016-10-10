#ifndef _POLYLOOP_GEOMETRY_PROVIDER_H_
#define _POLYLOOP_GEOMETRY_PROVIDER_H_

#include <memory>

#include <CGAL/circulator.h>

#include <OGRE/Ogre.h>
#include <OGRE/OgreSimpleRenderable.h>

#include "polyloop.h"

class PolyloopStripPolicy {
 public:
  static constexpr Ogre::RenderOperation::OperationType PRIMITIVE_TYPE =
      Ogre::RenderOperation::OT_LINE_STRIP;
};

// A geometry provider for the polyloop representation. The polyloop
// representation object itself must remain valid during the use of this
// adaptor class.
template <typename PointType, typename ProviderPolicy = PolyloopStripPolicy>
class PolyloopGeometryProvider : public ProviderPolicy {
 private:
  const Polyloop<PointType>* m_polyloop;
  using LoopCirculator = CGAL::Circulator_from_iterator<
      typename Polyloop<PointType>::const_iterator>;
  LoopCirculator m_circulator;
  using CirculatorContainer = CGAL::Container_from_circulator<LoopCirculator>;
  CirculatorContainer m_circularContainer;

 public:
  static constexpr int HINT_MAX_BOUND = Polyloop<PointType>::HINT_MAX_BOUND;
  using const_iterator = decltype(m_circularContainer.begin());

 public:
  PolyloopGeometryProvider(const Polyloop<PointType>& polyloop);
  PolyloopGeometryProvider(const Polyloop<PointType>&& polyloop) = delete;
  ~PolyloopGeometryProvider() {}

  size_t size() const { return m_polyloop->size() + 1; }

  auto begin() const -> decltype(std::declval<CirculatorContainer&>().begin()) {
    return m_circularContainer.begin();
  }
  auto end() const -> decltype(std::declval<CirculatorContainer&>().end()) {
    return m_circularContainer.end();
  }
};

template <typename PointType, typename ProviderPolicy>
PolyloopGeometryProvider<PointType, ProviderPolicy>::PolyloopGeometryProvider(
    const Polyloop<PointType>& polyloop)
    : m_polyloop(&polyloop),
      m_circulator(m_polyloop->begin(), m_polyloop->end()),
      m_circularContainer(m_circulator) {}

#endif  //_POLYLOOP_GEOMETRY_PROVIDER_H_
