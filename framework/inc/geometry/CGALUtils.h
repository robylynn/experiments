#ifndef _FRAMEWORK_GEOMETRY_CGAL_UTILS_H_
#define _FRAMEWORK_GEOMETRY_CGAL_UTILS_H_

#include <Eigen/Geometry>
#include <Eigen/Dense>

#include <CGAL/Vector_3.h>
#include <CGAL/aff_transformation_tags.h>

#include "geometryTypes.h"

// Utils functions for CGAL types.
class CGALUtils {
 public:
  // Compute a rotation matrix that rotates the z axis of the world coordinate
  // system so that it is aligned with the provided plane.
  static Kernel::Aff_transformation_3 RotationMatrixForPlane(
      const Kernel::Plane_3& plane) {
    Eigen::Quaternion<Kernel::FT> quat;
    Kernel::Vector_3 planeNormal = plane.orthogonal_vector();
    quat.setFromTwoVectors(
        Eigen::Vector3d(0, 0, 1),
        Eigen::Vector3d(planeNormal[0], planeNormal[1], planeNormal[2]));
    quat.normalize();
    Eigen::Matrix3d mat = quat.toRotationMatrix();
    return Kernel::Aff_transformation_3(mat(0, 0), mat(0, 1), mat(0, 2),
                                        mat(1, 0), mat(1, 1), mat(1, 2),
                                        mat(2, 0), mat(2, 1), mat(2, 2));
  }
};

#endif  //_FRAMEWORK_GEOMETRY_CGAL_UTILS_H_
