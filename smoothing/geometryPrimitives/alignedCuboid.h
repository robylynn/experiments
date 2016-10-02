#ifndef _GEOMPRIMITIVES_ALIGNED_CUBOID_H_
#define _GEOMPRIMITIVES_ALIGNED_CUBOID_H_

#include <CGAL/Triangle_3.h>

// The aligned cuboid triangle builder class builds a set of triangles the union
// of which make up the cuboid
class AlignedCuboidTriangleBuilder {
  // Lookup table according to CGAL IsoCuboid indexing scheme (docs.cgal.org)
  static constexpr int TRIANGLES_PER_CUBOID = 12;
  static constexpr int s_triangleLUT[TRIANGLES_PER_CUBOID][3] = {
      {0, 1, 2},  // Back
      {0, 2, 3},
      {0, 3, 4},  // Left
      {0, 4, 5},
      {2, 3, 7},  // Top
      {3, 4, 7},
      {4, 5, 6},  // Front
      {4, 6, 7},
      {1, 2, 7},  // Right
      {1, 7, 6},
      {0, 1, 6},  // Bottom
      {0, 6, 5}};

 public:
  AlignedCuboidTriangleBuilder(const Kernel::Iso_cuboid_3& cuboid)
      : m_cuboid(cuboid){};

  class TriangleIterator
      : public boost::iterator_facade<TriangleIterator, Kernel::Triangle_3> {
   public:
    TriangleIterator() : m_triangleIndex(0) {}

   private:
    friend class boost::iterator_core_access;

    void increment() {
      m_triangleIndex++;
      m_triangle = Triangle(s_triangleLUT[triangleIndex][0],
                            s_triangleLUT[m_triangleIndex][1],
                            s_triangleLUT[m_triangleIndex][2]);
    }

    bool equal(const AlignedCuboidTriangleIterator& other) {
      return (m_triangleIndex == other.m_triangleIndex);
    }

    const Kernel::Triangle_3& dereference() const { return m_triangle; }

    size_t m_triangleIndex;
    Kernel::Triangle_3 m_triangle;
  };

  Kernel::Iso_cuboid_3 cuboid m_cuboid;
};

#endif _GEOMPRIMITIVES_ALIGNED_CUBOID_H_
