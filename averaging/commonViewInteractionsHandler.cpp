#include "commonViewInteractionsHandler.h"

CommonViewInteractionsHandler::CommonViewInteractionsHandler(
    Ogre::SceneNode* parentNode)
    : m_rootNode(parentNode->createChildSceneNode()),
      m_fieldLevelSetVisualizer(m_rootNode) {}

void CommonViewInteractionsHandler::levelSetSliderChanged(
    const CEGUI::EventArgs& eventArgs) {
  m_fieldLevelSetVisualizer.setNormalizedLevel(
      static_cast<CEGUI::Slider*>(static_cast<const CEGUI::WindowEventArgs*>(
                                      &eventArgs)->window)->getCurrentValue());
}

void CommonViewInteractionsHandler::fieldChanged(
    const SquaredDistField* field) {
  m_fieldLevelSetVisualizer.setField(field);
}
