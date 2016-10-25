#include "geometryInterop.h"

Ogre::Vector3 GeometryInterop::renderingFromGeom(const Kernel::Vector_3& vector) {
  return Ogre::Vector3(vector.x(), vector.y(), vector.z());
}

Ogre::Vector3 GeometryInterop::renderingFromGeom(const Kernel::Point_3& point) {
  return Ogre::Vector3(point.x(), point.y(), point.z());
}

Ogre::AxisAlignedBox GeometryInterop::renderingFromGeom(
    const Kernel::Iso_cuboid_3& boundingBox) {
  return Ogre::AxisAlignedBox(renderingFromGeom(boundingBox.min()),
                              renderingFromGeom(boundingBox.max()));
}
