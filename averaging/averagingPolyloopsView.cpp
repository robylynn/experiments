#include <OGRE/OgreSceneManager.h>
#include <OGRE/OgreEntity.h>

#include <polyloop_3.h>
#include <defaultRenderables.h>

#include "averagingPolyloopsView.h"

AveragingPolyloopsView::AveragingPolyloopsView(Ogre::SceneNode* rootNode)
    : m_rootNode(rootNode) {
  Ogre::SceneNode* polyloopRootNode = m_rootNode->createChildSceneNode();

  Polyloop_3 p1;
  buildPolyloopFromObj("data/loop1.obj", p1);

  auto loopMeshable = make_mesh_renderable(p1, "polyloop1");
  Ogre::Entity* p1Entity =
      m_rootNode->getCreator()->createEntity("entityPolyloop1", "polyloop1");
  p1Entity->setMaterialName("Materials/DefaultLines");

  polyloopRootNode->attachObject(p1Entity);
  // Polyloop_2 p2D;
  // buildPolyloopFromObj("data/loop1_2D.obj", p2D);
}
