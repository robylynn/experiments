#ifndef _FIELD_SAMPLER_H_
#define _FIELD_SAMPLER_H_

#include "geometryTypes.h"

// Samples values of a field. The templated class expects a
// Sampler that generates sample points, a Field that is capable of
// being queried for sample points, and yields an iterator over <sample_point,
// field value>.
template <typename Field, typename SamplingIter>
class FieldSampler {
 public:
  using result_type = std::tuple<typename SamplingIter::value_type,
                                 typename Field::result_type>;

 private:
  using SamplingFunctionType = std::function<typename Field::result_type(
      typename SamplingIter::reference)>;
  using IterType = boost::transform_iterator<
      std::function<result_type(typename SamplingIter::reference)>,
      SamplingIter>;

 public:
  FieldSampler(SamplingFunctionType samplingFunction, const SamplingIter& begin,
               const SamplingIter& end)
      : m_samplingFunction(samplingFunction),
        m_sampleBegin(begin),
        m_sampleEnd(end) {}

  result_type operator()(typename SamplingIter::reference sample) const {
    return std::tuple<typename SamplingIter::value_type,
                      typename Field::result_type>(sample,
                                                   m_samplingFunction(sample));
  }

  IterType begin() const {
    return boost::make_transform_iterator(m_sampleBegin, *this);
  }

  IterType end() const {
    return boost::make_transform_iterator(m_sampleEnd, *this);
  }

 private:
  SamplingFunctionType m_samplingFunction;
  SamplingIter m_sampleBegin;
  SamplingIter m_sampleEnd;
};

#endif  //_FIELD_SAMPLER_H_
