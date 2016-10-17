#ifndef _PLANAR_STREAM_LINE_GENERATOR_H_
#define _PLANAR_STREAM_LINE_GENERATOR_H_

#include <CGAL/Stream_lines_2.h>
#include <CGAL/Regular_grid_2.h>

#include "geometryTypes.h"

// Visualizes a 3D vector field by generating stream lines at sample planes
// some degree apart at a point in E3.
template <typename VectorField>
class DefaultPlanarStreamLineGenerationPolicy {
 protected:
  using Integrator = CGAL::Runge_kutta_integrator_2<VectorField>;
  float m_samplingAngle = boost::math::constants::pi<double>() / 2;
};

class StreamLines {
  StreamLines();
  PositionOnlyBufferProvider<StreamLinesProvider>>
      getStreamLinesBufferProvider() {
    return PositionOnlyBufferProvider<>();
  }

  PositionOnlyBufferProvider<StreamLinesProvider>>
      getSamplePlanesBufferProvider() {
    return PositionOnlyBufferProvider<>()
  }

 private:
  std::vector<std::tuple<UniformPlanarGrid, CGAL::Stream_lines_2>>
      m_streamLines;
}

template <typename VectorField,
          typename GenerationPolicy = DefaultPlanarStreamLineGenerationPolicy>
class PlanarGridStreamLinesBuilder
    : protected GenerationPolicy
      PlanarGridStreamLinesBuilder(const Kernel::Point_3& point,
                         const VectorField& vectorField)

      // Generate stream lines using CGAL's stream line placement algorithm.
      // The algorithm "flows" a starting point over the vector field, sampling
      // using the separating distance parameter, till the stream line is far
      // from
      // others by the separating distance.
      // The entire space is kept under a Delaunay triangulation as well, whose
      // circumcenter center's are used to select the farthest point from the
      // currently placed streamlines (as long as such a new point selection is
      // possible under the user provided saturation ratio).
      void generateStreamLines(Kernel::FT separatingDistance,
                               Kernel::FT saturationRatio) {
};

#endif  //_PLANAR_STREAM_LINE_GENERATOR_H_
