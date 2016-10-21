#ifndef _SEPARABLE_GEOMETRY_INDUCED_FIELD_H_
#define _SEPARABLE_GEOMETRY_INDUCED_FIELD_H_

#include <boost/variant.hpp>
#include <boost/mpl/transform.hpp>

#include "geometryVariants.h"
#include "polyloop.h"
#include "uniformVoxelGrid.h"
#include "variantWrapper.h"

// A "separable" field in R3 that is induced by a group of geometric objects.
// Any field that is computable as a linear function of individually
// induced distance fields on each geometric object can be modeled as such.
// The geometry objects should remain valid as long as the field is queried.
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
template <typename Computer,
          typename GeometryTypesVariant = PointDistanceComputableTypes  //,
          /*typename Aggregator = UseDefault*/>
class SeparableGeometryInducedField {
  using GeometryReferenceTypesVariant =
      typename boost::make_variant_over<typename boost::mpl::transform<
          typename GeometryTypesVariant::types,
          ConstReferenceTypeWrapper<boost::mpl::_1>>::type>::type;

 public:
  using result_type = typename Computer::result_type;

  // TODO msati3: Fix initialization of GeometryInducedDistance field to make
  // parameters not dependent on extent, indexExtent, etc
  SeparableGeometryInducedField() {}

  // Add a geometry representation to the field. The callers must ensure that
  // the geometry representations remain valid till they are done using the
  // InducedField object's API's
  template <typename Representation>
  void addGeometry(const Representation& geometryRep) {
    addGeometryReference(std::cref(geometryRep));
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

  result_type operator()(const Kernel::Point_3& point) const {
    result_type sampledValue = std::accumulate(
        m_representations.begin(), m_representations.end(), result_type(0),
        [this, point, &sampledValue](const result_type& init,
                                     GeometryReferenceTypesVariant repRef) {
          Computer computer(point);
          WrappedVariantInvoker<Computer> invoker(computer);
          return init + boost::apply_visitor(invoker, repRef);
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
  void addGeometryReference(GeometryReferenceTypesVariant geometryRef) {
    m_representations.push_back(geometryRef);
  }

  std::vector<GeometryReferenceTypesVariant> m_representations;
  /*std::unique_ptr<Aggregator> m_aggregator;*/
};

#endif  // _SEPARABLE_GEOMETRY_INDUCED_FIELD_H_
