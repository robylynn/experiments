#include <psimpl.h>

#include <memory>

#include <containerAlgorithms.h>

#include "geometryTypes.h"
#include "polyline_3.h"
#include "simplicialAdaptors/polylineSimplicialAdaptor.h"
#include "defaultBufferProviders.h"
#include "simplification/polyline_3Simplifier.h"

template <typename Polyline_3>
std::tuple<size_t, Polyline_3> void douglasPeuckerSimplify(
    const Polyline_3& input) {
  Polyline_3 simplified;

  // Unroll data to flat form for use with psimpl's douglas peucker
  // algorithm
  using GeometryProvider =
      PolylineGeometryProvider<Polyline_3, PolylinePointPolicy>;
  using GeometryAdaptor =
      SingleElementProviderAdaptor<GeometryProvider, PositionVertexElement>;
  VertexElementBufferProvider<GeometryAdaptor, PositionVertexElement>
      flatIterProvider{GeometryProvider(input)};
  std::vector<float> simplifiedFlat;

  // Simplify polyline with tolerance.
  psimpl::simplify_douglas_peucker<3>(flatIterProvider.begin(),
                                      flatIterProvider.end(), m_tolerance,
                                      std::back_inserter(simplifiedFlat));

  // Create simplified polyline from unrolled points.
  for (auto iter =
           utils::tuple_iterator<decltype(simplifiedFlat.begin()), 3,
                                 Kernel::Point_3>::begin(simplifiedFlat.begin(),
                                                         simplifiedFlat.end());
       iter !=
           utils::tuple_iterator<decltype(simplifiedFlat.end()), 3,
                                 Kernel::Point_3>::end(simplifiedFlat.end());
       ++iter) {
    simplified.addPoint(*iter);
  }
  return {simplified.size(), simplified};
}
