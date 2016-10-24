#ifndef _WINDOWED_RENDERING_APP_H_
#define _WINDOWED_RENDERING_APP_H_

#include <memory>
#include <functional>

#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/Ogre/Renderer.h>
#include <OGRE/OgreFrameListener.h>
#include <OIS/OIS.h>

#include "notifier.h"

class InputSystemPoller {
 public:
  virtual ~InputSystemPoller() {}
  virtual bool poll() = 0;
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

  bool onMouseEvent(const std::string& name, const boost::any& parameters);

  OIS::InputManager* m_inputSystemManager;
  std::unique_ptr<Ogre::FrameListener> m_renderLoopInputListener;

  OIS::Mouse* m_mouse;
  OIS::Keyboard* m_keyboard;

  // The renderer that is provided to CEGUI -- we provide Ogre's rendering
  // interface for CEGUI for its widget rendering
  CEGUI::OgreRenderer* m_guiRenderer;
  std::unique_ptr<SubscriberRAIIWrapper> m_guiMouseEventSubscriber;

  std::unique_ptr<Ogre::Root> m_root;
  std::string m_name;
};

#endif  //_WINDOWED_RENDERING_APP_H_
