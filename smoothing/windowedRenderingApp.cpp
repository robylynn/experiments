#include <sstream>

#include <glog/logging.h>

#include <OGRE/Ogre.h>

#include "windowedRenderingApp.h"

WindowedRenderingApp::WindowedRenderingApp(const std::string& name)
    : m_root(new Ogre::Root()), m_name(name), m_inputSystemManager(nullptr) {}

WindowedRenderingApp::~WindowedRenderingApp() {
  m_inputSystemManager->destroyInputObject(m_mouse);
  m_inputSystemManager->destroyInputObject(m_keyboard);
  OIS::InputManager::destroyInputSystem(m_inputSystemManager);
}

// Initialize window - rendering, keyboard and mouse handling functionalities
bool WindowedRenderingApp::init(unsigned int width, unsigned int height) {
  // Initialize rendering
  if (!(m_root->showConfigDialog())) {
    LOG(INFO) << "Ogre rendering system failed to initialize";
    exit(-1);
    return false;
  }
  m_root->initialise(false);
  Ogre::RenderWindow* renderWindow =
      m_root->createRenderWindow(m_name, width, height, false /*fullScreen*/);

  // Initialize input
  OIS::ParamList paramList;
  size_t windowHandle = 0;
  std::ostringstream windowHandleStr;

  renderWindow->getCustomAttribute("WINDOW", &windowHandle);
  windowHandleStr << windowHandle;
  paramList.insert(
      std::make_pair(std::string("WINDOW"), windowHandleStr.str()));

// Non-fullscreen mode shouldn't capture mouse :(
#if VE_PLATFORM == VE_PLATFORM_LINUX
  paramList.insert({"x11_mouse_grab", "false"});
  paramList.insert({"x11_mouse_hide", "false"});
  paramList.insert({"x11_keyboard_grab", "false"});
  paramList.insert({"XAutoRepeatOn", "true"});
#elif VE_PLATFORM == VE_PLATFORM_WIN32
  paramList.insert({"w32_mouse", "DISCL_FOREGROUND"});
  paramList.insert({"w32_mouse", "DISCL_NONEXCLUSIVE"});
  paramList.insert({"w32_keyboard", "DISCL_FOREGROUND"});
  paramList.insert({"w32_keyboard", "DISCL_NONEXCLUSIVE"});
#endif

  m_inputSystemManager = OIS::InputManager::createInputSystem(paramList);
  m_renderLoopInputListener = new RenderLoopInputListener(*this);
  m_root->addFrameListener(m_renderLoopInputListener);

  m_keyboard = static_cast<OIS::Keyboard*>(
      m_inputSystemManager->createInputObject(OIS::OISKeyboard, true));
  m_keyboard->setEventCallback(&m_keyboardManager);

  m_mouse = static_cast<OIS::Mouse*>(
      m_inputSystemManager->createInputObject(OIS::OISMouse, true));
  m_mouse->setEventCallback(&m_mouseManager);

  const OIS::MouseState& mouseState = m_mouse->getMouseState();
  mouseState.width = width;
  mouseState.height = height;

  return true;
}

bool WindowedRenderingApp::poll() {
  m_mouse->capture();
  m_keyboard->capture();

  return m_keyboard->isKeyDown(OIS::KC_ESCAPE);
}
