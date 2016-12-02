#include "commonViewInteractionsHandler.h"

CommonViewInteractionsHandler::CommonViewInteractionsHandler(
    Ogre::SceneNode* parentNode)
    : m_rootNode(parentNode->createChildSceneNode()),
      m_levelSetNode(m_rootNode->createChildSceneNode()),
      m_heightFieldNode(m_rootNode->createChildSceneNode()),
      m_fieldLevelSetVisualizer(m_levelSetNode),
      m_heightFieldVisualizer(m_heightFieldNode) {}

void CommonViewInteractionsHandler::levelSetSliderChanged(
    const CEGUI::EventArgs& eventArgs) {
  m_fieldLevelSetVisualizer.setNormalizedLevel(
      static_cast<CEGUI::Slider*>(
          static_cast<const CEGUI::WindowEventArgs*>(&eventArgs)->window)
          ->getCurrentValue());
}

void CommonViewInteractionsHandler::fieldChanged(
    const SquaredDistField_3* inducedField) {
  m_fieldLevelSetVisualizer.setField(inducedField);
  m_heightFieldNode->setVisible(false);
  m_levelSetNode->setVisible(true);
}

void CommonViewInteractionsHandler::fieldChanged(
    const SquaredDistField_2* inducedField) {
  m_heightFieldVisualizer.setField(inducedField);
  m_levelSetNode->setVisible(false);
  m_heightFieldNode->setVisible(true);
}
