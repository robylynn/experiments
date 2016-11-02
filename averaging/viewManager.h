#ifndef _AVERAGING_VIEW_MANAGER_H_
#define _AVERAGING_VIEW_MANAGER_H_

#include <map>
#include <memory>

#include <OGRE/OgreSceneManager.h>

#include "averagingLinesView.h"
#include "averagingPolyloopsView.h"
#include "averagingVectorsView.h"
#include "commonViewInteractionsHandler.h"

enum class ViewType {
  VECTORS,
  LINES,
  LOOPS,
};

class ViewManager {
 public:
  ViewManager(Ogre::SceneManager* sceneManager)
      : m_sceneManager(sceneManager) {}

  void init();
  void averageVectorsView();
  void averageLinesView();
  void averagePolyloopsView();

 private:
  void switchView(ViewType viewType);

  Ogre::SceneManager* m_sceneManager;
  std::unique_ptr<AveragingVectorsView> m_vectorsView;
  std::unique_ptr<AveragingLinesView> m_linesView;
  std::unique_ptr<AveragingPolyloopsView> m_polyloopsView;
  std::unique_ptr<CommonViewInteractionsHandler> m_commonInteractionsHandler;
  std::map<ViewType, Ogre::SceneNode*> m_rootViewNodes;
};

#endif  //_AVERAGING_VIEW_MANAGER_H_
