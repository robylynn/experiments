#ifndef _AVERAGING_VIEW_MANAGER_H_
#define _AVERAGING_VIEW_MANAGER_H_

#include <map>
#include <memory>

#include <OGRE/OgreSceneManager.h>

#include "averagingLinesView.h"
#include "averagingPolyloops_3View.h"
#include "averagingPolyloops_2View.h"
#include "averagingVectorsView.h"
#include "commonViewInteractionsHandler.h"

enum class ViewType {
  VECTORS,
  LINES,
  LOOPS2,
  LOOPS3,
};

class ViewManager {
 public:
  ViewManager(Ogre::SceneManager* sceneManager);

  void init();
  void averageVectorsView();
  void averageLinesView();
  void averagePolyloops_3View();
  void averagePolyloops_2View();

 private:
  void switchView(ViewType viewType);

  Ogre::SceneManager* m_sceneManager;
  std::unique_ptr<AveragingVectorsView> m_vectorsView;
  std::unique_ptr<AveragingLinesView> m_linesView;
  std::unique_ptr<AveragingPolyloops_3View> m_polyloops_3View;
  std::unique_ptr<AveragingPolyloops_2View> m_polyloops_2View;
  std::unique_ptr<CommonViewInteractionsHandler> m_commonInteractionsHandler;
  std::map<ViewType, Ogre::SceneNode*> m_rootViewNodes;
};

#endif  //_AVERAGING_VIEW_MANAGER_H_
