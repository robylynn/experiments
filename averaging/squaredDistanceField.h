#ifndef _SQUARED_DISTANCE_FIELD_H_
#define _SQUARED_DISTANCE_FIELD_H_

#include "polyloop.h"
#include "uniformVoxelGrid.h"

// A distance field induced by a group of objects, samplable at samples on a
// uniform voxel grid. As the calculation of the distance field is a consuming
// operation, this should only be done when required, by invoking the
// sampleDistanceField function.
template <typename FieldSamplingPolicy>
class GeometryInducedDistanceField : public FieldSamplingPolicy {
 public:
  // TODO msati3: Fix initialization of GeometryInducedDistance field to make
  // parameters not dependent on extent, indexExtent, etc
  GeometryInducedDistanceField(FieldType extent, size_t indexExtent)
      : FieldSamplingPolicy(extent, indexExtent) {}

  void addPolyloop(const Polyloop<Kernel::Point_3>& polyloop) {
    m_polyloops.push_back(polyloop);
  }

  void sampleDistanceField() {
    resetDistanceFieldSampleValues();
    m_fieldSampleValues.reserve(this->size());
    for (const auto& gridPointIter : *this) {
      FieldType sampledValue = 0;
      for (const auto& polyloop : m_polyloops) {
        sampledValue += polyloop.get().computeDistance(gridPointIter);
      }
      m_fieldSampleValues.push_back(sampledValue);
    }
  }

  // Query the distance field along a point
  FieldType queryDistanceField(const Kernel::Point_3& point) {
    FieldType queryResult = 0;
    for (const auto& polyloop : m_polyloops) {
      queryResult += polyloop.get().computeDistance(point);
    }
    m_fieldSampleValues.push_back(queryResult);
    return queryResult;
  }

 private:
  void resetDistanceFieldSampleValues() { m_fieldSampleValues.clear(); }

  std::vector<std::reference_wrapper<const Polyloop<Kernel::Point_3>>>
      m_polyloops;
  std::vector<FieldType> m_fieldSampleValues;
};

#endif  //_SQUARED_DISTANCE_FIELD_H_
