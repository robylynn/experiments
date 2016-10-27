#include <iostream>
#include <memory>

#include <glog/logging.h>

#include <CGAL/Point_2.h>
#include <CGAL/Line_2.h>
#include <CGAL/Ray_2.h>
#include <CGAL/Segment_2.h>
#include <CGAL/Polygon_2.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Polyline_simplification_2/simplify.h>
#include <CGAL/Polyline_simplification_2/Squared_distance_cost.h>

using Kernel1 = CGAL::Exact_predicates_exact_constructions_kernel;
namespace PS = CGAL::Polyline_simplification_2;
typedef CGAL::Polygon_2<Kernel1> Polygon_2;

int main() {
  // PS::Stop_below_count_ratio_threshold stopThreshold(0.5);
  PS::Stop_above_cost_threshold stopThreshold(0.01);
  PS::Squared_distance_cost cost;
  Polygon_2 polygon;
  std::cin >> polygon;

  Polygon_2 simplified = PS::simplify(polygon, cost, stopThreshold);
  std::cout << polygon.size() << " " << simplified.size() << std::endl;
  std::for_each(simplified.vertices_begin(), simplified.vertices_end(),
                [](const CGAL::Point_2<Kernel1>& point) {
                  std::cout << point << std::endl;
                });
  return 0;
}
