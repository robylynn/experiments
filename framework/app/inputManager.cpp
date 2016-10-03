#include "inputManager.h"

bool MouseManager::mouseMoved(const OIS::MouseEvent& e) {
  // Populate with dummy mouse button pressed information
  MouseEventParams eventParams = std::make_pair(e, OIS::MB_Middle);
  notify(MOVED, std::cref(eventParams));
  return true;
}

bool MouseManager::mousePressed(const OIS::MouseEvent& e,
                                OIS::MouseButtonID id) {
  MouseEventParams eventParams = std::make_pair(e, id);
  notify(PRESSED, std::cref(eventParams));
  return true;
}

bool MouseManager::mouseReleased(const OIS::MouseEvent& e,
                                 OIS::MouseButtonID id) {
  MouseEventParams eventParams = std::make_pair(e, id);
  notify(RELEASED, std::cref(eventParams));
  return true;
}

bool KeyboardManager::keyPressed(const OIS::KeyEvent& e) {
  notify(PRESSED, std::cref(e));
  return true;
}
bool KeyboardManager::keyReleased(const OIS::KeyEvent& e) {
  notify(RELEASED, std::cref(e));
  return true;
}
