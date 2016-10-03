#ifndef _GEOMPRIMITIVES_ALIGNED_CUBOID_H_
#define _GEOMPRIMITIVES_ALIGNED_CUBOID_H_

#include <CGAL/Triangle_3.h>

// The aligned cuboid triangles builder class builds a set of triangles the
// union of which make up the cuboid. The builder is a read-only store of these
// triangles, serving only to generate them from an aligned cuboid, and thus,
// only const_iteration is possible.
class AlignedCuboidTrianglesBuilder {
 public:
  static constexpr int TRIANGLES_PER_CUBOID = 12;

  AlignedCuboidTrianglesBuilder(const Kernel::Iso_cuboid_3& cuboid)
      : m_cuboid(cuboid){};

  // ------------------------------------------------------------
  // Triangle Iterator specific code
  class TriangleIterator
      : public boost::iterator_facade<TriangleIterator, Kernel::Triangle_3,
                                      boost::forward_traversal_tag> {
   public:

    explicit TriangleIterator(
        const AlignedCuboidTrianglesBuilder* trianglesBuilder)
        : m_builder(trianglesBuilder), m_triangleIndex(0) {}

    TriangleIterator(const AlignedCuboidTrianglesBuilder* trianglesBuilder,
                     size_t triangleIndex)
        : m_builder(trianglesBuilder), m_triangleIndex(triangleIndex) {}

   private:
    friend class boost::iterator_core_access;

    void increment() { ++m_triangleIndex; }

    bool equal(const TriangleIterator& other) {
      return (m_triangleIndex == other.m_triangleIndex) &&
             (m_builder == other.m_builder);
    }

    // TODO msati3: Cache this is perf becomes an issue
    const Kernel::Triangle_3& dereference() const {
      m_triangle = Kernel::Triangle_3(
          m_builder->m_cuboid[s_triangleLUT[m_triangleIndex][0]],
          m_builder->m_cuboid[s_triangleLUT[m_triangleIndex][1]],
          m_builder->m_cuboid[s_triangleLUT[m_triangleIndex][2]]);
      return m_triangle;
    }

    const AlignedCuboidTrianglesBuilder* m_builder;
    size_t m_triangleIndex;
    // TODO msati3: Slight hack? Read. Mutable allows for simplified derefence
    // logic, while maintaining const correctness.
    mutable Kernel::Triangle_3 m_triangle;
  };

  using const_iterator = TriangleIterator;
  size_t size() const { return TRIANGLES_PER_CUBOID; }
  const_iterator begin() const { return const_iterator(this); }
  const_iterator end() const {
    return const_iterator(this, TRIANGLES_PER_CUBOID);
  }

  // End triangle iterator specific code
  // ------------------------------------------------------------
 private:
  Kernel::Iso_cuboid_3 m_cuboid;

  // Lookup table according to CGAL IsoCuboid indexing scheme (docs.cgal.org)
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
};

#endif  //_GEOMPRIMITIVES_ALIGNED_CUBOID_H_
