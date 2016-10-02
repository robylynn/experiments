#ifndef _POLYLOOP_GEOMETRY_PROVIDER_H_
#define _POLYLOOP_GEOMETRY_PROVIDER_H_

#include <memory>

#include <CGAL/circulator.h>

#include <OGRE/Ogre.h>
#include <OGRE/OgreSimpleRenderable.h>

#include "polyloop.h"

// A geometry provider for the polyloop representation. The polyloop
// representation object itself must remain valid during the use of this
// adaptor class.
template <typename T>
class PolyloopGeometryProvider {
 private:
  const Polyloop<T>& m_polyloop;
  using LoopCirculator =
      CGAL::Circulator_from_iterator<typename Polyloop<T>::const_iterator>;
  LoopCirculator m_circulator;
  CGAL::Container_from_circulator<LoopCirculator> m_circularContainer;

 public:
  static constexpr int HINT_MAX_BOUND = Polyloop<T>::HINT_MAX_BOUND;
  static constexpr Ogre::RenderOperation::OperationType PRIMITIVE_TYPE =
      Ogre::RenderOperation::OT_LINE_STRIP;

 public:
  PolyloopGeometryProvider(const Polyloop<T>& polyloop);
  ~PolyloopGeometryProvider() {}

  size_t size() const { return m_polyloop.size() + 1; }

  auto begin() const -> decltype(m_circularContainer.begin()) {
    return m_circularContainer.begin();
  }
  auto end() const -> decltype(m_circularContainer.end()) {
    return m_circularContainer.end();
  }
};

template <typename T>
PolyloopGeometryProvider<T>::PolyloopGeometryProvider(
    const Polyloop<T>& polyloop)
    : m_polyloop(polyloop),
      m_circulator(m_polyloop.begin(), m_polyloop.end()),
      m_circularContainer(m_circulator) {}

#endif  //_POLYLOOP_GEOMETRY_PROVIDER_H_
