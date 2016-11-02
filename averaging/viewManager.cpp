#include <glog/logging.h>

#include <OGRE/OgreSceneNode.h>

#include <windowedRenderingApp.h>

#include "viewManager.h"

void ViewManager::init() {
  // Add widgets and setup callbacks
  CEGUI::WindowManager& windowManager = CEGUI::WindowManager::getSingleton();
  CEGUI::Window* averagingLayout =
      windowManager.loadLayoutFromFile("Averaging.layout");
  CEGUI::GUIContext& guiContext =
      CEGUI::System::getSingleton().getDefaultGUIContext();
  guiContext.getRootWindow()->addChild(averagingLayout);

  averagingLayout->getChild("LevelSetSlider")
      ->subscribeEvent(
          CEGUI::Slider::EventThumbTrackEnded,
          CEGUI::Event::Subscriber(
              &CommonViewInteractionsHandler::levelSetSliderChanged,
              m_commonInteractionsHandler.get()));

  CEGUI::Window* navigationPane =
      averagingLayout->getChild("NavigationLinkContainer");

  navigationPane->getChild("AverageVectorsViewBtn")
      ->subscribeEvent(
          CEGUI::PushButton::EventClicked,
          CEGUI::Event::Subscriber(&ViewManager::averageVectorsView, this));
  navigationPane->getChild("AverageLinesViewBtn")
      ->subscribeEvent(
          CEGUI::PushButton::EventClicked,
          CEGUI::Event::Subscriber(&ViewManager::averageLinesView, this));
  navigationPane->getChild("AveragePolyloopsViewBtn")
      ->subscribeEvent(
          CEGUI::PushButton::EventClicked,
          CEGUI::Event::Subscriber(&ViewManager::averagePolyloopsView, this));
}

void ViewManager::averageVectorsView() {
  if (m_linesView == nullptr) {
    Ogre::SceneNode* vectorsNode =
        m_sceneManager->getRootSceneNode()->createChildSceneNode();
    m_vectorsView.reset(new AveragingVectorsView(vectorsNode));
    m_rootViewNodes.insert(std::make_pair(ViewType::VECTORS, vectorsNode));
  }
  switchView(ViewType::VECTORS);
}

void ViewManager::averageLinesView() {
  if (m_linesView == nullptr) {
    Ogre::SceneNode* linesNode =
        m_sceneManager->getRootSceneNode()->createChildSceneNode();
    m_linesView.reset(new AveragingLinesView(linesNode));
    m_rootViewNodes.insert(std::make_pair(ViewType::LINES, linesNode));
  }
  switchView(ViewType::LINES);
}

void ViewManager::averagePolyloopsView() {
  if (m_polyloopsView == nullptr) {
    Ogre::SceneNode* loopsNode =
        m_sceneManager->getRootSceneNode()->createChildSceneNode();
    m_polyloopsView.reset(new AveragingPolyloopsView(loopsNode));
    m_rootViewNodes.insert(std::make_pair(ViewType::LOOPS, loopsNode));
  }
  switchView(ViewType::LOOPS);
}

void ViewManager::switchView(ViewType viewType) {
  auto iter = m_rootViewNodes.find(viewType);
  LOG_IF(ERROR, iter == m_rootViewNodes.end())
      << "View switch requested not found";
  if (iter != m_rootViewNodes.end()) {
    // We have a valid view to switch to -- hide all other nodes.
    for (auto viewNodeTuple : m_rootViewNodes) {
      viewNodeTuple.second->setVisible(false);
    }
    iter->second->setVisible(true);
  }
}
