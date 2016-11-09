#ifndef _AVERAGING_COMMON_VIEW_INTERACTIONS_HANDLER_H_
#define _AVERAGING_COMMON_VIEW_INTERACTIONS_HANDLER_H_

#include <OGRE/OgreSceneNode.h>
#include <CEGUI/CEGUI.h>

#include "commonViewInterface.h"
#include "fieldVisualizers.h"

class CommonViewInteractionsHandler {
 public:
  CommonViewInteractionsHandler(Ogre::SceneNode* parentNode);
  void levelSetSliderChanged(const CEGUI::EventArgs& eventArg);
  void fieldChanged(const SquaredDistField* inducedField);

 private:
  Ogre::SceneNode* m_rootNode;
  LevelSetMeshVisualizer<SquaredDistField> m_fieldLevelSetVisualizer;
};

#endif  //_AVERAGING_COMMON_VIEW_INTERACTIONS_HANDLER_H_
