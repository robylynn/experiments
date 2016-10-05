#ifndef _CUBOID_GEOMETRY_PROVIDER_H_
#define _CUBOID_GEOMETRY_PROVIDER_H_

#include <CGAL/Triangle_3.h>

#include "geometryConstants.h"
#include "geometryTypes.h"

// TODO msati3: Switch to TrianglesBuilder with support for any class that is
// capable of providing a list of indices into a geometry data-structure.

// The aligned cuboid triangles builder class builds a set of triangles the
// union of which make up the cuboid. The builder is a read-only store of these
// triangles, serving only to generate them from an aligned cuboid, and thus,
// only const_iteration is possible.
class AlignedCuboidTrianglesBuilder {
 public:
  static constexpr int TRIANGLES_PER_CUBOID = 12;

  // Lookup table according to CGAL IsoCuboid indexing scheme (docs.cgal.org)
  static const int s_triangleLUT[TRIANGLES_PER_CUBOID][VERTICES_PER_TRIANGLE];

  AlignedCuboidTrianglesBuilder(const Kernel::Iso_cuboid_3& cuboid)
      : m_cuboid(cuboid){};

  // ------------------------------------------------------------
  // Triangle Iterator specific code
  class TriangleIterator
      : public boost::iterator_facade<TriangleIterator,
                                      const Kernel::Triangle_3,
                                      boost::random_access_traversal_tag> {
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

    bool equal(const TriangleIterator& other) const {
      return (m_triangleIndex == other.m_triangleIndex) &&
             (m_builder == other.m_builder);
    }

    TriangleIterator::difference_type distance_to(
        const TriangleIterator& other) {
      return other.m_triangleIndex - m_triangleIndex;
    }

    // TODO msati3: Cache this if perf becomes an issue
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
};

#endif  //_CUBOID_GEOMETRY_PROVIDER_H_
