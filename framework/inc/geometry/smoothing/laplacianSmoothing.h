#ifndef _FRAMEWORK_GEOMETRY_SMOOTHING_LAPLACIAN_SMOOTHING_H_
#define _FRAMEWORK_GEOMETRY_SMOOTHING_LAPLACIAN_SMOOTHING_H_

#include <Eigen/Core>

#include "polyline.h"
#include "polylineGeometryProvider.h"
#include "defaultBufferProviders.h"

template <typename PointType>
Polyline<PointType> laplacianSmoothing(const Polyline<PointType>& polyline,
                                       float stepSize, size_t numIterations) {
  Eigen::MatrixXf laplacian =
      Eigen::MatrixXf::Zero(polyline.size(), polyline.size());

  // Populate a single row of the laplace matrix by using an adjacency stencil
  auto laplaceStencilFill = [&laplacian](int index) {
    // Keep ends fixed.
    if (index > 0 && index < laplacian.cols() - 1) {
      laplacian.row(index)[index] = -2;
      laplacian.row(index)[index - 1] = 1;
      laplacian.row(index)[index + 1] = 1;
    }
  };

  for (size_t i = 0; i < polyline.size(); ++i) {
    laplaceStencilFill(i);
  }

  using GeometryProvider =
      PolylineGeometryProvider<Polyline<PointType>, PolylinePointPolicy>;
  using GeometryAdaptor =
      SingleElementProviderAdaptor<GeometryProvider, PositionVertexElement>;
  VertexElementBufferProvider<GeometryAdaptor, PositionVertexElement>
      flatIterProvider{GeometryProvider(polyline)};

  std::vector<float> unrolledPolyline;
  unrolledPolyline.insert(unrolledPolyline.begin(), flatIterProvider.begin(),
                          flatIterProvider.end());

  Eigen::Map<
      Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>>
      polylineMatrix(unrolledPolyline.data(), polyline.size(), 3);

  for (int i = 0; i < numIterations; ++i) {
    // One motion towards the neighbor average.
    polylineMatrix = polylineMatrix + stepSize * laplacian * polylineMatrix;
    // One motion away from the neighbor average.
    polylineMatrix = polylineMatrix - 0.5 * stepSize * laplacian * polylineMatrix;
  }

  // Create smoothed polyline from unrolled points.
  Polyline<PointType> smoothed;
  for (auto iter = utils::tuple_iterator<
           decltype(unrolledPolyline.begin()), 3,
           Kernel::Point_3>::begin(unrolledPolyline.begin(),
                                   unrolledPolyline.end());
       iter !=
           utils::tuple_iterator<decltype(unrolledPolyline.end()), 3,
                                 Kernel::Point_3>::end(unrolledPolyline.end());
       ++iter) {
    smoothed.addPoint(*iter);
  }
  return smoothed;
}

#endif  // _FRAMEWORK_GEOMETRY_SMOOTHING_LAPLACIAN_SMOOTHING_H_
