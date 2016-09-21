#ifndef _WINDOWED_RENDERING_APP_H_
#define _WINDOWED_RENDERING_APP_H_

#include <glog/logging.h>

#include <OGRE/OgreFrameListener.h>
#include <OIS/OIS.h>

#include "inputManager.h"
#include "notifier.h"

class InputSystemPoller {
 public:
  virtual ~InputSystemPoller() {}
  virtual bool poll() = 0;
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
// TODO msati3: Currently, input handling is performed within the render loop.
// Moving this outside will require tighter integration with some cross plat
// windowing library (FLTK?)
class WindowedRenderingApp : public InputSystemPoller {
 public:
  WindowedRenderingApp(const std::string& name);
  ~WindowedRenderingApp();
  bool init(unsigned int width, unsigned int height);
  void startEventLoop();

  bool poll() override;

  // Provide access to app wide services
  const std::string& getWindowName() const { return m_name; }
  NotificationsManager& getNotificationsManager() {
    return m_notificationsManager;
  }

 private:
  NotificationsManager m_notificationsManager;

  OIS::InputManager* m_inputSystemManager;
  RenderLoopInputListener* m_renderLoopInputListener;

  OIS::Mouse* m_mouse;
  OIS::Keyboard* m_keyboard;

  std::unique_ptr<Ogre::Root> m_root;
  std::string m_name;
};

#endif  //_WINDOWED_RENDERING_APP_H_
