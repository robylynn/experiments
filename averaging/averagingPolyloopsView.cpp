#include <OGRE/OgreSceneManager.h>
#include <OGRE/OgreEntity.h>

#include <polyloop_3.h>
#include <defaultRenderables.h>

#include "averagingPolyloopsView.h"

constexpr int NUM_LOOPS = 2;

AveragingPolyloopsView::AveragingPolyloopsView(Ogre::SceneNode* rootNode)
    : m_rootNode(rootNode),
      m_polyloopsRootNode(m_rootNode->createChildSceneNode()) {}

void AveragingPolyloopsView::populateData() {
  m_squaredDistField.reset(new SquaredDistField());
  for (int i = 0; i < NUM_LOOPS; ++i) {
    std::string loopName = "loop" + std::to_string(i);
    Polyloop_3 p;
    buildPolyloopFromObj("data/" + loopName + ".obj", p);
    if (i == 0) m_squaredDistField->addGeometry(p);

    auto loopMeshable = make_mesh_renderable(p, loopName);
    Ogre::Entity* pEntity = m_rootNode->getCreator()->createEntity(loopName);
    pEntity->setMaterialName("Materials/DefaultLines");
    m_polyloopsRootNode->attachObject(pEntity);
  }
}
