#ifndef _INPUT_MANAGER_H_
#define _INPUT_MANAGER_H_

#include <OIS/OIS.h>

#include "notifier.h"

typedef std::pair<const OIS::MouseEvent&, OIS::MouseButtonID> MouseEventParams;
typedef OIS::KeyEvent KeyboardEventParams;

class MouseManager : public OIS::MouseListener, public Notifier {
 public:
  static constexpr const char* MOVED = "mouseMoved";
  static constexpr const char* PRESSED = "mousePressed";
  static constexpr const char* RELEASED = "mouseReleased";

  static constexpr const char* NOTIFIER_NAME = "MouseManager";

  bool mouseMoved(const OIS::MouseEvent& e) {
    // Populate with dummy mouse button pressed information
    MouseEventParams eventParams = std::make_pair(e, OIS::MB_Middle);
    notify(MOVED, std::cref(eventParams));
    return true;
  }

  bool mousePressed(const OIS::MouseEvent& e, OIS::MouseButtonID id) {
    MouseEventParams eventParams = std::make_pair(e, id);
    notify(PRESSED, std::cref(eventParams));
    return true;
  }

  bool mouseReleased(const OIS::MouseEvent& e, OIS::MouseButtonID id) {
    MouseEventParams eventParams = std::make_pair(e, id);
    notify(RELEASED, std::cref(eventParams));
    return true;
  }
};

class KeyboardManager : public OIS::KeyListener,
                        public Notifier {
 public:
  static constexpr const char* PRESSED = "keyPressed";
  static constexpr const char* RELEASED = "keyReleased";

  static constexpr const char* NOTIFIER_NAME = "KeyboardManager";

  bool keyPressed(const OIS::KeyEvent& e) {
    notify(PRESSED, std::cref(e));
    return true;
  }
  bool keyReleased(const OIS::KeyEvent& e) {
    notify(RELEASED, std::cref(e));
    return true;
  }
};

#endif  //_INPUT_MANANGER_H_
