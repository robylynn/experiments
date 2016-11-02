#include <OGRE/OgreSceneManager.h>
#include <OGRE/OgreEntity.h>

#include <CGAL/Segment_3.h>

#include <geometryTypes.h>
#include <defaultRenderables.h>

#include "averagingVectorsView.h"

AveragingVectorsView::AveragingVectorsView(Ogre::SceneNode* rootNode)
    : m_rootNode(rootNode) {
  Ogre::SceneNode* vectorsRootNode = m_rootNode->createChildSceneNode();

  std::vector<Kernel::Point_3> v1Backing{Kernel::Point_3(0, 0, 0),
                                         Kernel::Point_3(1, 0, 0)};
  Kernel::Segment_3 v1(v1Backing[0], v1Backing[2]);

  auto v1Meshable = make_mesh_renderable(v1Backing, "vector1");
  Ogre::Entity* v1Entity =
      m_rootNode->getCreator()->createEntity("entityLine1", "vector1");
  v1Entity->setMaterialName("Materials/DefaultLines");
}
