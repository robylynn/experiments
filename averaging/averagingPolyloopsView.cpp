#include <OGRE/OgreSceneManager.h>
#include <OGRE/OgreEntity.h>

#include <polyloop_3.h>
#include <defaultRenderables.h>

#include "averagingPolyloopsView.h"
#include "gradientComputer.h"

constexpr int NUM_LOOPS = 2;
constexpr int MAX_ITERS = 3;

namespace {
// Compute snapping by gradient computation
Kernel::Point_3 snapNumerical(const Kernel::Point_3& point,
                                const SquaredDistField& field, int iterCount) {
  if (iterCount == MAX_ITERS) return point;
  NaiveGradientEstimator estimator(0.05);
  return snapNumerical(point, field, iterCount + 1);
}

Polyloop_3 snapAverage(const std::vector<Polyloop_3>& loops) {
  SquaredDistField squaredDistField;
  for (const auto& loop : loops) {
    squaredDistField.addGeometry(loop);
  }

  Polyloop_3 averageInit = loops[0];
  Polyloop_3 average;
  for (auto& point : averageInit) {
    average.addPoint(snapNumerical(point, squaredDistField, 0));
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
