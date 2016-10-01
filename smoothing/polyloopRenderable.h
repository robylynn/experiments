#ifndef _POLYLOOP_GEOMETRY_PROVIDER_H_
#define _POLYLOOP_GEOMETRY_PROVIDER_H_

#include <memory>

#include <CGAL/circulator.h>

#include <OGRE/Ogre.h>
#include <OGRE/OgreSimpleRenderable.h>

#include "polyloop.h"

// A geometry provider for the polyloop representation. The representation
// itself must remain valid during the use of this adaptor class.
template <typename T>
class PolyloopGeometryProvider {
 private:
  /*using LoopCirculator = CGAL::Circulator_from_iterator<
      typename Polyloop<T>::PointsContainer::iterator>;
  LoopCirculator m_circulator;
  CGAL::Container_from_circulator<LoopCirculator> m_circularContainer;*/
  const Polyloop<T>& m_polyloop;

 public:
  static constexpr int HINT_MAX_BOUND = Polyloop<T>::HINT_MAX_BOUND;
  static constexpr Ogre::RenderOperation::OperationType PRIMITIVE_TYPE =
      Ogre::RenderOperation::OT_LINE_STRIP;

 public:
  PolyloopGeometryProvider(const Polyloop<T>& polyloop);
  ~PolyloopGeometryProvider() {}

  size_t size() const { return m_polyloop.size(); }

  auto begin() const -> decltype(m_polyloop.begin()) {
    return m_polyloop.begin();
  }
  auto end() const -> decltype(m_polyloop.begin()) { return m_polyloop.end(); }
};

template <typename T>
PolyloopGeometryProvider<T>::PolyloopGeometryProvider(
    const Polyloop<T>& polyloop)
    : m_polyloop(polyloop) {}
/*m_circulator(polyloop.begin(), polyloop.end()),
m_circularContainer(m_circulator) {}*/

#endif  //_POLYLOOP_GEOMETRY_PROVIDER_H_
