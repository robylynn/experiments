#ifndef _FRAMEWORK_GEOMETRY_SAMPLERS_ORDERED_POINTS_ON_CIRCLE_SAMPLER_H_
#define _FRAMEWORK_GEOMETRY_SAMPLERS_ORDERED_POINTS_ON_CIRCLE_SAMPLER_H_

#include <boost/iterator/iterator_facade.hpp>

#include <CGAL/Point_3.h>
#include <CGAL/Vector_3.h>
#include <CGAL/Circle_3.h>
#include <CGAL/aff_transformation_tags.h>

#include "CGALUtils.h"
#include "geometryConstants.h"
#include "geometryTypes.h"

// A circle point generator generates points on circle. The points are placed
// at uniform increments of theta angles, and are ordered along the circle. The
// starting location is non-deterministic however.
//
// The points generated are not guaranteed to lie on the circle, due to
// normalized squared root computations, and should be used with care.
class OrderedPointsOnCircleSampler {
 public:
  OrderedPointsOnCircleSampler(const CGAL::Circle& circle, size_t numSamples)
      : m_circle(circle),
        m_numSamples(numSamples),
        m_radius(sqrt(m_circle->squared_radius())),
        m_rotation(CGALUtils::RotationMatrixForPlane(
            circle.supporting_plane().orthogonal_vector())) {}

  class PointGenerator
      : public boost::iterator_facade<PointGenerator, const Kernel::Point_3,
                                      std::input_iterator_tag,
                                      const Kernel::Point_3> {
   public:
    PointGenerator(const CircleSampler* provider, int sampleNum)
        : m_provider(provider), m_sampleNum(sampleNum) {}

   private:
    friend class boost::iterator_core_access;

    void increment() { m_sampleNum++; }

    bool equal(const PointGenerator& other) const {
      return m_sampleNum == other.m_sampleNum;
    }

    const Kernel::Point_3 dereference() const {
      Kernel::FT theta = m_sampleNum * (2 * PI / m_provider->m_numSamples);
      Kernel::Vector_3 planarVector(cos(theta), sin(theta), 0);
      Kernel::Vector_3 vector =
          m_provider->m_radius * m_provider->m_rotation(planarVector);
      Kernel::Point_3 samplePoint = m_provider->m_center + vector;
      return samplePoint;
    }

   private:
    mutable int m_sampleNum;
    const CircleSampler* m_provider;
  };

  using const_iterator = PointGenerator;

  const_iterator begin() const { return const_iterator(this, 0); }

  const_iterator end() const { return const_iterator(this, m_numSamples); }

  size_t size() { return m_numSamples; }

 private:
  Kernel::Circle_3 m_circle;
  // Cache the radius, as the Kernel::Circle_3 doesn't provide this.
  Kernel::FT m_radius;
  Kernel::Aff_transformation_3 m_rotation;
};

using OrderedPointsOnCircleGenerator = OrderedPointsOnCircleSampler;

#endif  //_FRAMEWORK_GEOMETRY_SAMPLER_ORDERED_POINTS_ON_CIRCLE_SAMPLER_H_
