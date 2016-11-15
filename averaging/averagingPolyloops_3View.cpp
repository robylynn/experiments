#include <Eigen/Eigenvalues>

#include <OGRE/OgreSceneManager.h>
#include <OGRE/OgreEntity.h>
#include <OGRE/OgreSubEntity.h>

#include <appContext.h>
#include <dynamicMeshManager.h>
#include <polyloop_3.h>

#include "averagingPolyloops_3View.h"
#include "hessianComputer.h"

constexpr int NUM_LOOPS = 2;
constexpr int MAX_ITERS = 10;

namespace Context = Framework::AppContext;

namespace {
// Compute snapping by gradient computation
class NumericalSnapper {
 public:
  NumericalSnapper(const SquaredDistField& distField,
                   float stepSize = s_stepSize,
                   float numericalGridSize = s_gridSize)
      : m_stepSize(stepSize),
        m_estimator(numericalGridSize),
        m_computer(distField, m_estimator) {}

  Kernel::Point_3 snap(const Kernel::Point_3& point, int iterCount) const {
    Eigen::Matrix3f hessian = m_computer(point);
    Eigen::SelfAdjointEigenSolver<Eigen::Matrix3f> solver(hessian);
    auto largestEigen = solver.eigenvectors().col(2) * solver.eigenvalues()[2];
    Kernel::Vector_3 vector(largestEigen[0], largestEigen[1], largestEigen[2]);
    return point - vector * s_stepSize;
  }

 private:
  float m_stepSize;
  NaiveHessianEstimator m_estimator;
  HessianComputer<SquaredDistField> m_computer;

  static constexpr float s_stepSize = 0.05;
  static constexpr float s_gridSize = 0.05;
};

// A snap algorithm for averaging can be stated as:
// Start with an initial set of sample points.
// For each sample point, make some local adjustments towards average
// direction.
// Do the above process iteratively.
class SnapAverage {
 public:
  SnapAverage(Ogre::SceneNode* parentNode,
              float normalizedConvergenceThreshold = s_perVConvergenceThreshold)
      : m_parentNode(parentNode),
        m_normalizedConvergenceThreshold(normalizedConvergenceThreshold) {}

  Polyloop_3 operator()(const std::vector<Polyloop_3>& loops) {
    SquaredDistField squaredDistField;
    for (const auto& loop : loops) {
      squaredDistField.addGeometry(loop);
    }

    NumericalSnapper snapper(squaredDistField);
    Polyloop_3 averageCurrent = loops[0];
    bool converged = false;
    for (int i = 0; i < MAX_ITERS && !converged; ++i) {
      Polyloop_3 average;
      for (auto& point : averageCurrent) {
        average.addPoint(snapper.snap(point, 0));
      }
      visualizeSnapTrajectory(averageCurrent, average);
      converged = checkConvergence(averageCurrent, average);
      averageCurrent = average;
    }
    return averageCurrent;
  }

  bool checkConvergence(const Polyloop_3& current, const Polyloop_3& snapped) {
    // Capture the difference between current and snapped positions, using it
    // to determine convergence.
    Kernel::FT diffPrevious = 0;
    for (auto iterCur = current.begin(), iterSnap = snapped.begin();
         iterCur != current.end(); ++iterSnap, ++iterCur) {
      diffPrevious =
          std::max(diffPrevious, CGAL::squared_distance(*iterCur, *iterSnap));
    }
    diffPrevious = sqrt(diffPrevious);
    return (diffPrevious < m_normalizedConvergenceThreshold);
  }

  void visualizeSnapTrajectory(const Polyloop_3& current,
                               const Polyloop_3& snapped) {
    LOG_IF(ERROR, current.size() != snapped.size())
        << "The snapped trajectory does not have the same number of points as "
           "the initial trajectory";
    std::vector<Kernel::Point_3> snapVectors;
    for (auto iterCur = current.begin(), iterSnap = snapped.begin();
         iterCur != current.end(); ++iterSnap, ++iterCur) {
      snapVectors.push_back(*iterCur);
      snapVectors.push_back(*iterSnap);
    }
    Ogre::Entity* pEntitySnapAvg =
        Context::getDynamicMeshManager().addMesh(snapped, m_parentNode);
    pEntitySnapAvg->setMaterialName("Materials/DefaultLines");
    pEntitySnapAvg->getSubEntity(0)
        ->setCustomParameter(1, Ogre::Vector4(0, 0, 1, 1));

    Ogre::Entity* pEntitySnapVectors =
        Context::getDynamicMeshManager().addMesh(snapVectors, m_parentNode);
    pEntitySnapVectors->setMaterialName("Materials/DefaultLines");
    pEntitySnapVectors->getSubEntity(0)
        ->setCustomParameter(1, Ogre::Vector4(0, 0, 1, 1));

    Ogre::Entity* pEntitySnapNeighborhoodField = Context::getDynamicMeshManager().addMesh
  }

 private:
  Ogre::SceneNode* m_parentNode;
  float m_normalizedConvergenceThreshold;

  static constexpr float s_perVConvergenceThreshold = 0.0002;
};
}  // end anon namespace

AveragingPolyloops_3View::AveragingPolyloops_3View(Ogre::SceneNode* rootNode)
    : m_rootNode(rootNode),
      m_polyloopsRootNode(m_rootNode->createChildSceneNode()) {}

void AveragingPolyloops_3View::populateData() {
  // m_squaredDistField.reset(new SquaredDistField());
  std::vector<Polyloop_3> loops;
  for (int i = 0; i < NUM_LOOPS; ++i) {
    std::string loopName = "loop" + std::to_string(i);
    loops.push_back(Polyloop_3());
    buildPolyloopFromObj("data/" + loopName + ".obj", loops[loops.size() - 1]);
    // m_squaredDistField->addGeometry(loops[loops.size()-1]);

    Ogre::Entity* pEntity = Context::getDynamicMeshManager().addMesh(
        loops[loops.size() - 1], m_polyloopsRootNode);
    pEntity->setMaterialName("Materials/DefaultLines");
  }

  SnapAverage snapAverage(m_polyloopsRootNode);
  Polyloop_3 average = snapAverage(loops);
  Ogre::Entity* pEntity =
      Context::getDynamicMeshManager().addMesh(average, m_polyloopsRootNode);
  pEntity->setMaterialName("Materials/AveragePolyloops");
}
