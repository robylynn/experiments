#ifndef _SEPARABLE_GEOMETRY_INDUCED_FIELD_H_
#define _SEPARABLE_GEOMETRY_INDUCED_FIELD_H_

#include <boost/variant.hpp>

#include "polyloop.h"
#include "uniformVoxelGrid.h"

// A "separable" field in R3 that is induced by a group of geometric objects.
// Any field that is computable as a linear function of individually
// induced distance fields on each geometric object can be modeled as such.
//
// The field is sampled along any FieldSampler that is capable of being
// iterated over, each iteration yielding a point in R3.
//
// The InducedFieldType is the type of the induced field representation.
// Examples include a scalar field, and a vector field. The Computer must
// provide the InducedFieldType as typedef ComputedFieldType
//
// The computation of the distance field itself is carried out
// by a Computer that, given a point sample in R3 and a primitive,
// computes the field value due to the primitive at the sample. A computer must
// derive from FieldComputer;
//
// The aggregation of the computed values for each primitive is performed by
// the Aggregator.
template <typename GeometryTypesVariant, typename Computer  //,
          /*typename Aggregator = UseDefault*/>
class SeparableGeometryInducedField {
  using InducedFieldType = typename Computer::ComputedFieldType;

 public:
  // TODO msati3: Fix initialization of GeometryInducedDistance field to make
  // parameters not dependent on extent, indexExtent, etc
  SeparableGeometryInducedField() {}

  void addGeometry(const GeometryTypesVariant& geometryRep) {
    m_representations.push_back(geometryRep);
  }

  /*void setAggregator(std::unique_ptr<Aggregator> aggregator) {
    m_aggregator.reset(std::move(aggregator));
  }*/

  /*void pointSample(const Kernel::Point_3& pointSample) {
    InducedFieldType sampledValue = std::accumulate(
        m_representations.begin(), m_representations.end(), InducedFieldType(0),
        [&this, &pointSample ](const InducedFieldType& init, const auto& rep) {
          return m_aggregator(m_computer(rep, point));
        });
  }*/

  InducedFieldType pointSample(const Kernel::Point_3& point) {
    InducedFieldType sampledValue = std::accumulate(
        m_representations.begin(), m_representations.end(), InducedFieldType(0),
        [this, point, &sampledValue](const InducedFieldType& init,
                                     const GeometryTypesVariant& rep) {
          Computer computer(point);
          return init + boost::apply_visitor(computer, rep);
        });
    return sampledValue;
  }

  /*template <typename SamplingIterator>
  OutputIterator sampleDistanceField(const SamplingIterator& begin,
                                     const SamplingIterator& end) {
    std::for_each(begin, end, []() { InducedFieldType fieldValue = pSampled });
  }*/

 private:
  /*void resetDistanceFieldSampleValues() { m_fieldSampleValues.clear(); }*/

  std::vector<std::reference_wrapper<const GeometryTypesVariant>>
      m_representations;
  /*std::unique_ptr<Aggregator> m_aggregator;*/
};

#endif  // _SEPARABLE_GEOMETRY_INDUCED_FIELD_H_
