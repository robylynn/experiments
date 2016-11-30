#ifndef _AVERAGING_COMMON_VIEW_INTERACTIONS_HANDLER_H_
#define _AVERAGING_COMMON_VIEW_INTERACTIONS_HANDLER_H_

#include <CEGUI/CEGUI.h>
#include <OGRE/OgreSceneNode.h>

#include "commonViewInterface.h"
#include "fieldLevelSetVisualizer.h"
#include "heightFieldVisualizer.h"

class CommonViewInteractionsHandler {
 public:
  CommonViewInteractionsHandler(Ogre::SceneNode* parentNode);
  void levelSetSliderChanged(const CEGUI::EventArgs& eventArg);

  void fieldChanged(const SquaredDistField_3* inducedField);

  void fieldChanged(const SquaredDistField_2* inducedField);

 private:
  Ogre::SceneNode* m_rootNode;
  Ogre::SceneNode* m_levelSetNode;
  Ogre::SceneNode* m_heightFieldNode;
  LevelSetMeshVisualizer<SquaredDistField_3> m_fieldLevelSetVisualizer;
  HeightFieldVisualizer<SquaredDistField_2> m_heightFieldVisualizer;
};

#endif  //_AVERAGING_COMMON_VIEW_INTERACTIONS_HANDLER_H_
