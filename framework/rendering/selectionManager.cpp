#include <queue>

#include <OGRE/OgreSceneManager.h>

#include "selectionManager.h"

void SelectionManager::selectionQuery(float mouseX, float mouseY) {
  Ogre::Ray selectionRay = m_camera->getCameraToViewportRay(mouseX, mouseY);
  Ogre::RaySceneQuery* rayQuery =
      m_camera->getSceneManager()->createRayQuery(Ogre::Ray());
  rayQuery->setRay(selectionRay);
  rayQuery->setSortByDistance(false);

  Ogre::RaySceneQueryResult& result = rayQuery->execute();

  // Finer queries for SelectableObjects whose bounding box is intersected
  // by the ray-scene query. The query mechanism ensures that post a
  // queryDistance call, a setSelected call will be received. This
  using QueueElementType = std::tuple<float, ISelectableObject*>;
  std::priority_queue<QueueElementType, std::vector<QueueElementType>,
                      std::greater<QueueElementType>> distanceQueue;
  for (auto it = result.begin(); it != result.end(); ++it) {
    if (m_selectableObjectsMap.find(it->movable) !=
        m_selectableObjectsMap.end()) {
      ISelectableObject* selectableObject = m_selectableObjectsMap[it->movable];
      distanceQueue.push(std::make_tuple(
          selectableObject->queryDistance(selectionRay), selectableObject));
    }
  }
  std::get<1>(distanceQueue.top())->setSelected(selectionRay);
}
