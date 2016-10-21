#include <sstream>

#include <glog/logging.h>

#include <OGRE/Ogre.h>

#include "windowedRenderingApp.h"
#include "inputManager.h"

namespace {

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

}  // end anon-namespace

// Global app instance
WindowedRenderingApp* g_appInstance = nullptr;

NotificationsManager& getAppWideNotificationsManager() {
  return g_appInstance->getNotificationsManager();
}

WindowedRenderingApp::WindowedRenderingApp(const std::string& name)
    : m_root(new Ogre::Root()), m_name(name), m_inputSystemManager(nullptr) {
  g_appInstance = this;
}

WindowedRenderingApp::~WindowedRenderingApp() {
  m_inputSystemManager->destroyInputObject(m_mouse);
  m_inputSystemManager->destroyInputObject(m_keyboard);
  OIS::InputManager::destroyInputSystem(m_inputSystemManager);
}

// Initialize window - rendering, keyboard and mouse handling functionalities
bool WindowedRenderingApp::init(unsigned int width, unsigned int height) {
  // Initialize rendering
  if (!(m_root->showConfigDialog())) {
    LOG(ERROR) << "Ogre rendering system failed to initialize";
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
  // Enable mouse grab, as that makes zooming work
  // paramList.insert({"x11_mouse_grab", "false"});
  paramList.insert({"x11_mouse_hide", "false"});
  paramList.insert({"x11_keyboard_grab", "false"});
  paramList.insert({"XAutoRepeatOn", "true"});
#elif VE_PLATFORM == VE_PLATFORM_WIN32
  paramList.insert({"w32_mouse", "DISCL_FOREGROUND"});
  paramList.insert({"w32_mouse", "DISCL_NONEXCLUSIVE"});
  paramList.insert({"w32_keyboard", "DISCL_FOREGROUND"});
  paramList.insert({"w32_keyboard", "DISCL_NONEXCLUSIVE"});
#endif

  Ogre::ConfigFile cf;
  cf.load("resources.cfg");

  Ogre::ConfigFile::SectionIterator secIt = cf.getSectionIterator();
  while (secIt.hasMoreElements()) {
    Ogre::ConfigFile::SettingsMultiMap* settings = secIt.getNext();
    for (auto iter = settings->begin(); iter != settings->end(); ++iter) {
      Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
          iter->second, iter->first);
    }
  }
  Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

  m_inputSystemManager = OIS::InputManager::createInputSystem(paramList);
  m_renderLoopInputListener.reset(new RenderLoopInputListener(*this));
  m_root->addFrameListener(m_renderLoopInputListener.get());

  m_keyboard = static_cast<OIS::Keyboard*>(
      m_inputSystemManager->createInputObject(OIS::OISKeyboard, true));
  std::shared_ptr<KeyboardManager> keyboardManager(new KeyboardManager());
  m_keyboard->setEventCallback(keyboardManager.get());
  m_notificationsManager.registerNotifier(KeyboardManager::NOTIFIER_NAME,
                                          keyboardManager);

  m_mouse = static_cast<OIS::Mouse*>(
      m_inputSystemManager->createInputObject(OIS::OISMouse, true));
  std::shared_ptr<MouseManager> mouseManager(new MouseManager());
  m_mouse->setEventCallback(mouseManager.get());
  m_notificationsManager.registerNotifier(MouseManager::NOTIFIER_NAME,
                                          mouseManager);

  const OIS::MouseState& mouseState = m_mouse->getMouseState();
  mouseState.width = width;
  mouseState.height = height;

  return true;
}

void WindowedRenderingApp::startEventLoop() {
  // TODO msati3: Perhaps switch from OGRE's rendering loop later.
  m_root->startRendering();
}

bool WindowedRenderingApp::poll() {
  m_mouse->capture();
  m_keyboard->capture();

  return m_keyboard->isKeyDown(OIS::KC_ESCAPE);
}
