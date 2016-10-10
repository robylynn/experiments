#ifndef _SPHERE_GEOMETRY_PROVIDER_H_
#define _SPHERE_GEOMETRY_PROVIDER_H_

#include <CGAL/Sphere_3.h>
#include <CGAL/Polyhedron_3.h>

#include "geometryTypes.h"

// Generate points on a unit sphere that may be used as a triangle mesh
// representation.
class SphereTrianglesBuilder {
  SphereTrianglesBuilder(const Kernel::Sphere3& sphere) : m_sphere(sphere) {}

 private:
  Kernel::Sphere_3 m_sphere;
};

#endif  //_SPHERE_GEOMETRY_PROVIDER_H_
