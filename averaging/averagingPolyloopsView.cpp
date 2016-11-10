#include <OGRE/OgreSceneManager.h>
#include <OGRE/OgreEntity.h>

#include <polyloop_3.h>
#include <defaultRenderables.h>

#include "averagingPolyloopsView.h"
#include "hessianComputer.h"

constexpr int NUM_LOOPS = 2;
constexpr int MAX_ITERS = 1;

namespace {
// Compute snapping by gradient computation
class NumericalSnapper {
 public:
  NumericalSnapper(const SquaredDistField& distField)
      : m_estimator(0.05), m_computer(distField, m_estimator) {}

  Kernel::Point_3 snap(const Kernel::Point_3& point, int iterCount) const {
    if (iterCount == MAX_ITERS) return point;

    Eigen::Matrix3f hessian = m_computer(point);
    Eigen::EigenSolver<Eigen::Matrix3f> solver(hessian);
    solver.eigenvectors().col(0);
    return snap(point, iterCount + 1);
  }

 private:
  NaiveHessianEstimator m_estimator;
  HessianComputer<SquaredDistField> m_computer;
};

Polyloop_3 snapAverage(const std::vector<Polyloop_3>& loops) {
  SquaredDistField squaredDistField;
  for (const auto& loop : loops) {
    squaredDistField.addGeometry(loop);
  }

  NumericalSnapper snapper(squaredDistField);
  Polyloop_3 averageInit = loops[0];
  Polyloop_3 average;
  for (auto& point : averageInit) {
    average.addPoint(snapper.snap(point, 0));
  }

  return average;
}
}  // end anon namespace

AveragingPolyloopsView::AveragingPolyloopsView(Ogre::SceneNode* rootNode)
    : m_rootNode(rootNode),
      m_polyloopsRootNode(m_rootNode->createChildSceneNode()) {}

void AveragingPolyloopsView::populateData() {
  // m_squaredDistField.reset(new SquaredDistField());
  std::vector<Polyloop_3> loops;
  for (int i = 0; i < NUM_LOOPS; ++i) {
    std::string loopName = "loop" + std::to_string(i);
    loops.push_back(Polyloop_3());
    buildPolyloopFromObj("data/" + loopName + ".obj", loops[loops.size() - 1]);
    // m_squaredDistField->addGeometry(loops[loops.size()-1]);

    (void)make_mesh_renderable(loops[loops.size() - 1], loopName);
    Ogre::Entity* pEntity = m_rootNode->getCreator()->createEntity(loopName);
    pEntity->setMaterialName("Materials/DefaultLines");
    m_polyloopsRootNode->attachObject(pEntity);
  }

  Polyloop_3 average = snapAverage(loops);
  (void)make_mesh_renderable(average, "loopAverage");
  Ogre::Entity* pEntity = m_rootNode->getCreator()->createEntity("loopAverage");
  pEntity->setMaterialName("Materials/AveragePolyloops");
  m_polyloopsRootNode->attachObject(pEntity);
}
