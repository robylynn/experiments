#include <math.h>

#include <glog/logging.h>

#include <Eigen/Dense>

#include <containerAlgorithms.h>

#include "geometryConstants.h"
#include "geometryTypes.h"
#include "polyline.h"
#include "simplification/polyline_3Simplifier.h"

using Polyline_3 = Polyline<Kernel::Point_3>;

// Check if the set of family of planes that share the line through being and
// end and have the distance between all points between being and end less than
// the prescribed tolerance is non-empty? Is so, the result is a tuple of bool,
// and two planes that bound the wedge.
using FindWedgeResult = std::tuple<bool, Kernel::Plane_3, Kernel::Plane_3>;
template <typename PointIter>
FindWedgeResult findWedgeForTolerance(PointIter begin, PointIter end,
                                      Kernel::FT tolerance) {
  assert(begin != end);
  DLOG_IF(ERROR, *begin == *end)
      << "FindWedgeForTolerance - fed in iterators where the first and last "
         "dereferenced points are the same. This could actually represent an "
         "entire circle, instead of a degenerate case of all points being "
         "coincident and has to be properly handled. Now would be a good time "
         "to write code that handles this case.";

  bool wedgeFound = false;
  Kernel::Line_3 wedgePivotLine(*begin, *end);
  Kernel::Plane_3 referencePlane = wedgePivotLine.perpendicular_plane(*begin);
  Kernel::Plane_3 wedgePlane1 = referencePlane;
  Kernel::Plane_3 wedgePlane2 = referencePlane;

  // Must be that all points are colinear. Returning two same planes to
  // indicate that the wedge includes all planes.
  if (referencePlane.is_degenerate()) {
    return std::make_tuple(true, wedgePlane1, wedgePlane2);
  }
  Kernel::Vector_3 normedPlaneVect =
      referencePlane.orthogonal_vector() /
      sqrt(referencePlane.orthogonal_vector().squared_length());

  std::array<Kernel::FT, 2> validWedgeAngles{-M_PI, M_PI};
  for (auto iter = begin; iter != end; ++iter) {
    Kernel::Plane_3 curPointPlane = Kernel::Plane_3(*begin, *end, *iter);
    // Is the current query plane is degenerate, we can use the default (all of
    // (-PI,PI)) value of valid angles, as this point is coincident with the
    // begin and end points. Else, we compute.
    if (!curPointPlane.is_degenerate()) {
      Kernel::FT distToWedgePivot =
          sqrt(CGAL::squared_distance(*iter, wedgePivotLine));
      // If the ball around current point of radius tolerance intersects the
      // wedgePivot line, then, any point of intersection is always < tolerance
      // away from the point, and is always in any plane that passes through
      // the wedgePivot line. Thus, keep the validWedgeAngle unchanged for this
      // case. Else, we compute.
      if (tolerance >= distToWedgePivot) {
        Kernel::FT ratio =
            utils::clamp<Kernel::FT>(tolerance / distToWedgePivot, -1.0, 1.0);
        Kernel::FT angleAboutPointPlane = abs(asin(ratio));

        Kernel::FT pointPlaneReferenceInnerProd =
            (curPointPlane.orthogonal_vector() * normedPlaneVect) /
            sqrt(curPointPlane.orthogonal_vector().squared_length());
        pointPlaneReferenceInnerProd =
            utils::clamp(pointPlaneReferenceInnerProd, -1.0, 1.0);
        Kernel::FT pointPlaneReferenceAngle =
            acos(pointPlaneReferenceInnerProd);

        validWedgeAngles[0] =
            std::max(validWedgeAngles[0],
                     pointPlaneReferenceAngle - angleAboutPointPlane);
        validWedgeAngles[1] =
            std::min(validWedgeAngles[1],
                     pointPlaneReferenceAngle + angleAboutPointPlane);

        // Early exit if no interval overlap already. No wedge is found in this
        // case.
        if (validWedgeAngles[0] > validWedgeAngles[1]) {
          // std::cout << "\t\t\tNo valid wedge found " << validWedgeAngles[0]
          //          << " " << validWedgeAngles[1] << std::endl;
          return std::make_tuple(wedgeFound, wedgePlane1, wedgePlane2);
        }
      }
    }
  }
  wedgeFound = true;
  Kernel::Vector_3 unitWedgePivotVect = wedgePivotLine.to_vector();
  unitWedgePivotVect =
      unitWedgePivotVect / sqrt(unitWedgePivotVect.squared_length());

  auto rotateAboutAxis = [](const Kernel::Vector_3& vector,
                            const Kernel::Vector_3& axis, Kernel::FT angle) {
    Eigen::AngleAxisf rotation(Eigen::AngleAxisf(
        angle, Eigen::Vector3f(axis.x(), axis.y(), axis.z())));
    Eigen::Vector3f rotated =
        rotation * Eigen::Vector3f(vector.x(), vector.y(), vector.z());
    return Kernel::Vector_3(rotated[0], rotated[1], rotated[2]);
  };

  Kernel::Vector_3 wedgePlane1Normal =
      rotateAboutAxis(normedPlaneVect, unitWedgePivotVect, validWedgeAngles[0]);
  Kernel::Vector_3 wedgePlane2Normal =
      rotateAboutAxis(normedPlaneVect, unitWedgePivotVect, validWedgeAngles[1]);
  return std::make_tuple(wedgeFound, Kernel::Plane_3(*begin, wedgePlane1Normal),
                         Kernel::Plane_3(*begin, wedgePlane2Normal));
}

template <typename PointIter>
std::tuple<bool, std::vector<Kernel::Point_3>> findCircleFit(
    PointIter begin, PointIter end, const Kernel::Plane_3& searchPlane,
    Kernel::FT tolerance) {
  // Precondition -- there must be atleast three points for a circle fit.
  assert(begin + 1 != end);

  Kernel::FT sqTolerance = tolerance * tolerance;
  std::vector<Kernel::Point_3> simplifiedSamples;
  // Check if a line works.
  Kernel::Segment_3 endPointsSegment(*begin, *end);
  bool fLineValid = true;
  for (auto iter = begin + 1; iter != end && fLineValid; ++iter) {
    Kernel::FT squaredDist = CGAL::squared_distance(endPointsSegment, *iter);
    if (squaredDist > sqTolerance) {
      fLineValid = false;
      // std::cout << "\t\t\tTolerance exceeded for line by " << *iter << " "
      //          << squaredDist << std::endl;
    }
  }
  if (fLineValid) {
    simplifiedSamples.push_back(*begin);
    simplifiedSamples.push_back(*end);
    // std::cout << "\t\tFit line between " << *begin << " " << *end <<
    // std::endl;
    return std::make_tuple(true, simplifiedSamples);
  }

  bool fValidFitFound = false;
  for (auto iter = begin + 1; iter != end && !fValidFitFound; ++iter) {
    std::vector<Kernel::Point_3> simplifiedSamplesTry;
    if (!CGAL::collinear(*begin, *iter, *end)) {
      Kernel::Circle_3 circleCandidate(*begin, *iter, *end);
      Kernel::FT circleRadius = sqrt(circleCandidate.squared_radius());
      bool fCircleValid = true;
      for (auto iter = begin + 1; iter != end - 1 && fCircleValid; ++iter) {
        Kernel::Point_3 pointCirclePlaneProjection =
            circleCandidate.supporting_plane().projection(*iter);
        Kernel::FT squaredDist =
            CGAL::squared_distance(pointCirclePlaneProjection, *iter) +
            (CGAL::squared_distance(circleCandidate.center(),
                                    pointCirclePlaneProjection) -
             circleCandidate.squared_radius());
        if (squaredDist > sqTolerance) {
          // std::cout << "\t\t\tTolerance exceeded for circle c:"
          //          << circleCandidate.center() << " r:" << circleRadius
          //          << " by " << *iter << " " << squaredDist << std::endl;
          fCircleValid = false;
        } else {
          Kernel::Vector_3 unitCenterSegment(circleCandidate.center(),
                                             pointCirclePlaneProjection);
          unitCenterSegment =
              unitCenterSegment / sqrt(unitCenterSegment.squared_length());
          simplifiedSamplesTry.push_back(circleCandidate.center() +
                                         unitCenterSegment * circleRadius);
        }
      }
      fValidFitFound = fCircleValid;
      if (fValidFitFound) {
        // std::cout << "\t\tFit circle c:" << circleCandidate.center()
        //          << " r:" << circleRadius << " between " << *begin << " "
        //          << *end << std::endl;
        simplifiedSamples = std::move(simplifiedSamplesTry);
        break;  // We are done searching as we have found 'a' fit -- this may
                // not be optimal however. TODO msati3: Is this hacky?
      }
    }
  }
  return std::tuple<bool, std::vector<Kernel::Point_3>>{fValidFitFound,
                                                        simplifiedSamples};
}

// Given a start point, try fitting as large a circle as possible (that goes
// through as many points upto end), so that points in between are all at max
// tolerance distance away from the plane.
template <typename PointIter>
std::tuple<bool, std::vector<Kernel::Point_3>, PointIter> greedyFitCircle(
    PointIter begin, PointIter end, float tolerance) {
  Kernel::Circle_3 dummyCircle(Kernel::Point_3(1, 0, 0),
                               Kernel::Point_3(0, 1, 0),
                               Kernel::Point_3(-1, 0, 0));
  std::vector<Kernel::Point_3> simplifiedSamples;
  simplifiedSamples.push_back(*begin);
  simplifiedSamples.push_back(*end);
  // Try increments of powers of two, to size largest range of points where a
  // circle fits. Return with a line fit if the greedy fit is just requested for
  // two consecutive points.
  std::tuple<bool, std::vector<Kernel::Point_3>, PointIter> retVal{
      true, simplifiedSamples, begin + 1};

  // A single step remains. This will be a circle of infinite radius (line).
  if (begin + 2 == end) {
    // std::cout << "\t\tFit line as last segment left " << std::endl;
    return retVal;
  }

  // We start with a found wedge -- the one that just leads to a straight line
  // segment between the first and next point.
  bool fFoundWedgeThisIter = true;
  size_t curIncrement = 1;
  PointIter curEnd = begin + curIncrement;
  bool fAtEnd = false;

  while (fFoundWedgeThisIter && !fAtEnd) {
    fFoundWedgeThisIter = false;
    // Advance from current position by curIncrement
    for (size_t numIncrements = 0;
         (numIncrements < curIncrement) && (curEnd != end);
         ++numIncrements, ++curEnd) {
    };
    fAtEnd = (curEnd == end);
    PointIter fitEnd = fAtEnd ? curEnd - 1 : curEnd;
    // std::cout << "\tTry fitting from " << *begin << " to " << *fitEnd
    //          << std::endl;
    auto wedgeResult = findWedgeForTolerance(begin, fitEnd, tolerance);
    // Stop searching when the next iteration does not yield a valid wedge.
    if (std::get<0>(wedgeResult)) {
      Kernel::Plane_3 wedgeBisector =
          CGAL::bisector(std::get<1>(wedgeResult), std::get<2>(wedgeResult));
      auto bestCircleFit =
          findCircleFit(begin, fitEnd, wedgeBisector, tolerance);
      if (std::get<0>(bestCircleFit)) {
        retVal = std::make_tuple(std::get<0>(bestCircleFit),
                                 std::move(std::get<1>(bestCircleFit)), fitEnd);
        fFoundWedgeThisIter = true;
      }
    }
    curIncrement *= 2;
  }
  return retVal;
}

std::tuple<size_t, Polyline_3> Polyline_3Simplifier::simplify(
    const Polyline_3& input,
    const Polyline_3SimplificationStrategyNaiveBiarc& /**/) {
  auto begin = input.begin();
  auto end = input.end();

  Polyline_3 simplifiedLine;
  size_t numPrimitivesSimplified = 0;
  std::tuple<bool, std::vector<Kernel::Point_3>, Polyline_3::const_iterator>
      fitResult;
  do {
    // std::cout << "Will begin next sample from " << *begin << std::endl;
    fitResult = greedyFitCircle(begin, end, m_tolerance);
    begin = std::get<2>(fitResult);
    DLOG_IF(ERROR, !std::get<0>(fitResult)) << "Could not fit any valid "
                                               "primitive? This should not "
                                               "happen without a buggy code";
    std::vector<Kernel::Point_3>& simplifiedSamples = std::get<1>(fitResult);
    for (auto iter = simplifiedSamples.begin(); iter != simplifiedSamples.end();
         ++iter) {
      simplifiedLine.addPoint(*iter);
    }
    numPrimitivesSimplified++;
  } while (begin + 1 != end);
  return std::make_tuple(numPrimitivesSimplified, simplifiedLine);
}
