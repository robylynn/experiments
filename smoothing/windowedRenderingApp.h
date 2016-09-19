#ifndef _WINDOWED_RENDERING_APP_H_
#define _WINDOWED_RENDERING_APP_H_

#include <OIS/OIS.h>
#include <OGRE/OgreFrameListener.h>

class InputSystemPoller {
 public:
  virtual ~InputSystemPoller() {}
  virtual bool poll() = 0;
};

class MouseManager : public OIS::MouseListener {
 public:
  bool mouseMoved(const OIS::MouseEvent& e) { return true; }
  bool mousePressed(const OIS::MouseEvent& e, OIS::MouseButtonID id) {
    return true;
  }
  bool mouseReleased(const OIS::MouseEvent& e, OIS::MouseButtonID id) {
    return true;
  }
};

class KeyboardManager : public OIS::KeyListener {
 public:
  bool keyPressed(const OIS::KeyEvent& e) { return true; }
  bool keyReleased(const OIS::KeyEvent& e) { return true; }
};

class RenderLoopInputListener : public Ogre::FrameListener {
 public:
  RenderLoopInputListener(InputSystemPoller& poller)
      : m_inputSystemPoller(poller) {}

  bool frameStarted(const Ogre::FrameEvent& event) {
    return !m_inputSystemPoller.poll();
  }

  bool frameEnded(const Ogre::FrameEvent& event) { return true; }

 private:
  InputSystemPoller& m_inputSystemPoller;
};

// Support for a single window application. The window houses the mouse and
// keyboard listeners that can forward events to objects subscribing to them.
// TODO msati3: Hacked together right now. Figure the design over time.
// TODO msati3: Currently, input handling is performed within the render loop.
// Moving this outside will require tighter integration with some cross plat
// windowing library (FLTK?)
class WindowedRenderingApp : public InputSystemPoller {
 public:
  WindowedRenderingApp(const std::string& name);
  ~WindowedRenderingApp();
  bool init(unsigned int width, unsigned int height);

  bool poll() override;

  const std::string& getWindowName() const { return m_name; }

 private:
  OIS::InputManager* m_inputSystemManager;
  RenderLoopInputListener* m_renderLoopInputListener;

  OIS::Mouse* m_mouse;
  MouseManager m_mouseManager;

  OIS::Keyboard* m_keyboard;
  KeyboardManager m_keyboardManager;

  std::unique_ptr<Ogre::Root> m_root;
  std::string m_name;
};

#endif  //_WINDOWED_RENDERING_APP_H_
