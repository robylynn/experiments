#include <OGRE/OgreSceneManager.h>
#include <OGRE/OgreEntity.h>

#include <polyloop_3.h>
#include <defaultRenderables.h>

#include "averagingPolyloopsView.h"

constexpr int NUM_LOOPS = 2;

AveragingPolyloopsView::AveragingPolyloopsView(Ogre::SceneNode* rootNode)
    : m_rootNode(rootNode) {
  Ogre::SceneNode* polyloopRootNode = m_rootNode->createChildSceneNode();

  for (int i = 0; i < NUM_LOOPS; ++i) {
    std::string loopName = "loop" + std::to_string(i);
    Polyloop_3 p;
    buildPolyloopFromObj("data/"+loopName+".obj", p);

    auto loopMeshable = make_mesh_renderable(p, loopName);
    Ogre::Entity* pEntity =
      m_rootNode->getCreator()->createEntity(loopName);
    pEntity->setMaterialName("Materials/DefaultLines");

    polyloopRootNode->attachObject(pEntity);
    // Polyloop_2 p2D;
    // buildPolyloopFromObj("data/loop1_2D.obj", p2D);
  }
}
