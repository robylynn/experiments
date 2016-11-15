#include <glog/logging.h>

#include <OGRE/OgreSceneNode.h>

#include <windowedRenderingApp.h>

#include "viewManager.h"

ViewManager::ViewManager(Ogre::SceneManager* sceneManager)
    : m_sceneManager(sceneManager) {}

void ViewManager::init() {
  // Populate common interaction handler
  m_commonInteractionsHandler.reset(new CommonViewInteractionsHandler(
      m_sceneManager->getRootSceneNode()->createChildSceneNode()));

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
  navigationPane->getChild("AveragePolyloops2ViewBtn")
      ->subscribeEvent(
          CEGUI::PushButton::EventClicked,
          CEGUI::Event::Subscriber(&ViewManager::averagePolyloops_2View, this));
  navigationPane->getChild("AveragePolyloops3ViewBtn")
      ->subscribeEvent(
          CEGUI::PushButton::EventClicked,
          CEGUI::Event::Subscriber(&ViewManager::averagePolyloops_3View, this));
}

void ViewManager::averageVectorsView() {
  if (m_vectorsView == nullptr) {
    Ogre::SceneNode* vectorsNode =
        m_sceneManager->getRootSceneNode()->createChildSceneNode();
    m_rootViewNodes.insert(std::make_pair(ViewType::VECTORS, vectorsNode));
    m_vectorsView.reset(new AveragingVectorsView(vectorsNode));
  }
  m_vectorsView->populateData();
  m_commonInteractionsHandler->fieldChanged(
      m_vectorsView->getGeometryInducedField());

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

void ViewManager::averagePolyloops_2View() {
  if (m_polyloops_2View == nullptr) {
    Ogre::SceneNode* loopsNode =
        m_sceneManager->getRootSceneNode()->createChildSceneNode();
    m_polyloops_2View.reset(new AveragingPolyloops_2View(loopsNode));
    m_rootViewNodes.insert(std::make_pair(ViewType::LOOPS2, loopsNode));
  }
  m_polyloops_2View->populateData();
  m_commonInteractionsHandler->fieldChanged(
      m_polyloops_2View->getGeometryInducedField());
  switchView(ViewType::LOOPS2);
}

void ViewManager::averagePolyloops_3View() {
  if (m_polyloops_3View == nullptr) {
    Ogre::SceneNode* loopsNode =
        m_sceneManager->getRootSceneNode()->createChildSceneNode();
    m_polyloops_3View.reset(new AveragingPolyloops_3View(loopsNode));
    m_rootViewNodes.insert(std::make_pair(ViewType::LOOPS3, loopsNode));
  }
  m_polyloops_3View->populateData();
  m_commonInteractionsHandler->fieldChanged(
      m_polyloops_3View->getGeometryInducedField());
  switchView(ViewType::LOOPS3);
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
