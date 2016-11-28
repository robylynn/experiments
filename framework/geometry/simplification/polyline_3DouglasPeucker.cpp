#include <psimpl.h>

#include <memory>

#include <containerAlgorithms.h>

#include "defaultBufferProviders.h"
#include "geometryTypes.h"
#include "polyline.h"
#include "polylineGeometryProvider.h"
#include "simplification/polyline_3Simplifier.h"

using Polyline_3 = Polyline<Kernel::Point_3>;

std::tuple<size_t, Polyline_3> Polyline_3Simplifier::simplify(
    const Polyline_3& input,
    const Polyline_3SimplificationStrategyDouglasPeucker& /**/) {
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
  return std::make_tuple(simplified.size(), simplified);
}
