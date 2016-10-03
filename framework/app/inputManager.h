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

  bool mouseMoved(const OIS::MouseEvent& e);
  bool mousePressed(const OIS::MouseEvent& e, OIS::MouseButtonID id);
  bool mouseReleased(const OIS::MouseEvent& e, OIS::MouseButtonID id);
};

class KeyboardManager : public OIS::KeyListener, public Notifier {
 public:
  static constexpr const char* PRESSED = "keyPressed";
  static constexpr const char* RELEASED = "keyReleased";

  static constexpr const char* NOTIFIER_NAME = "KeyboardManager";

  bool keyPressed(const OIS::KeyEvent& e);
  bool keyReleased(const OIS::KeyEvent& e);
};

#endif  //_INPUT_MANANGER_H_
