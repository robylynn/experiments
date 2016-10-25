#ifndef _FRAMEWORK_RENDERING_GEOMETRY_INTEROP_H_
#define _FRAMEWORK_RENDERING_GEOMETRY_INTEROP_H_

// Some convenience helpers to interop between geometry representations and
// rendering implementations (CGAL and Ogre)

#include <CGAL/Iso_cuboid_3.h>

#include <OGRE/OgreAxisAlignedBox.h>

#include "geometryTypes.h"

class GeometryInterop {
 public:
  static Ogre::AxisAlignedBox renderingFromGeom(
      const Kernel::Iso_cuboid_3& boundingBox);

  static Ogre::Vector3 renderingFromGeom(const Kernel::Vector_3& vector);

  // In Ogre, Vector3 is used to represent both a point and a vector
  static Ogre::Vector3 renderingFromGeom(const Kernel::Point_3& point);
};

#endif  //_FRAMEWORK_RENDERING_GEOMETRY_INTEROP_H_
