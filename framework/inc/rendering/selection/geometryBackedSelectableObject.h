#ifndef _FRAMEWORK_RENDERING_GEOMETRY_BACKED_SELECTABLE_OBJECT_H_
#define _FRAMEWORK_RENDERING_GEOMETRY_BACKED_SELECTABLE_OBJECT_H_

#include <CGAL/squared_distance_2.h>
#include <OGRE/OgreSceneNode.h>

#include "geometryTypes.h"
#include "squaredDistance.h"
#include "selectableObject.h"

class DefaultSelectableMouseHandler {};

class DefaultSelectableKeyboardHandler {};

// A GeometryBackedSelectable object is a selectable object that is backed by a
// custom geometry representation. It implements its queries by forwarding to
// the geometry representation.
template <typename GeometryRep,
          typename MouseEventHandler = DefaultSelectableMouseHandler,
          typename KeyboardHandler = DefaultSelectableKeyboardHandler>
class GeometryBackedSelectableObject : public ISelectableObject {
 public:
  GeometryBackedSelectableObject(Ogre::MovableObject* movableObject)
      : m_movable(movableObject) {}

  float queryDistance(const Ogre::Ray& oray) override {
    Kernel::Ray_3 ray(
        Kernel::Point_3(oray.getOrigin().x, oray.getOrigin().y,
                        oray.getOrigin().z),
        Kernel::Vector_3(oray.getDirection().x, oray.getDirection().y,
                         oray.getDirection().z));
    return CGAL::squared_distance(*m_geometryRep, ray);
  }

  Ogre::MovableObject* movableObject() const { return m_movable; }

  void setSelected(const Ogre::Ray& oray) override {
    m_movable->getParentSceneNode()->showBoundingBox(true);
  }

 private:
  Ogre::MovableObject* m_movable;
  GeometryRep* m_geometryRep;
};

#endif  // _FRAMEWORK_RENDERING_GEOMETRY_BACKED_SELECTABLE_OBJECT_H_
