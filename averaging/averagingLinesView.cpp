#include <OGRE/OgreSceneManager.h>
#include <OGRE/OgreEntity.h>

#include <CGAL/Segment_3.h>

#include <geometryTypes.h>
#include <defaultRenderables.h>

#include "averagingLinesView.h"

AveragingLinesView::AveragingLinesView(Ogre::SceneNode* rootNode)
    : m_rootNode(rootNode) {
  Ogre::SceneNode* linesRootNode = m_rootNode->createChildSceneNode();

  std::vector<Kernel::Point_3> l1Backing{Kernel::Point_3(0, 0, 0),
                                         Kernel::Point_3(1, 0, 0)};
  Kernel::Segment_3 l1(l1Backing[0], l1Backing[2]);

  auto l1Meshable = make_mesh_renderable(l1Backing, "line1");
  Ogre::Entity* l1Entity =
      m_rootNode->getCreator()->createEntity("entityLine1", "line1");
  l1Entity->setMaterialName("Materials/DefaultLines");
}
