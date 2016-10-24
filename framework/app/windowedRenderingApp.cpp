#include <sstream>

#include <glog/logging.h>

#include <OGRE/Ogre.h>
#include <CEGUI/CEGUI.h>

#include "inputManager.h"
#include "notifier.h"
#include "windowedRenderingApp.h"

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
    std::string sectionName = secIt.peekNextKey();
    Ogre::ConfigFile::SettingsMultiMap* settings = secIt.getNext();
    for (auto iter = settings->begin(); iter != settings->end(); ++iter) {
      Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
          iter->second, iter->first, sectionName);
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

  // Set up the CEGUI system
  m_guiRenderer =
      &CEGUI::OgreRenderer::bootstrapSystem(*m_root->getRenderTarget(m_name));
  CEGUI::ImageManager::setImagesetDefaultResourceGroup("ImageSets");
  CEGUI::Font::setDefaultResourceGroup("Fonts");
  CEGUI::Scheme::setDefaultResourceGroup("Schemes");
  CEGUI::WidgetLookManager::setDefaultResourceGroup("LookNFeel");
  CEGUI::WindowManager::setDefaultResourceGroup("Layouts");

  // Set some default look, font and mouse arrow
  CEGUI::SchemeManager::getSingleton().createFromFile("TaharezLook.scheme");
  CEGUI::System::getSingleton().getDefaultGUIContext().setDefaultFont(
      "DejaVuSans-12");
  CEGUI::System::getSingleton()
      .getDefaultGUIContext()
      .getMouseCursor()
      .setDefaultImage("TaharezLook/MouseArrow");

  // Create root window for CEGUI. Any layout, etc are parented to this
  CEGUI::WindowManager& windowManager = CEGUI::WindowManager::getSingleton();
  CEGUI::Window* rootWindow =
      windowManager.createWindow("DefaultWindow", "rootWin");
  // Events received to the DefaultWindow are passed thru, allowing
  // application to handle it.
  rootWindow->setMousePassThroughEnabled(true);
  CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(
      rootWindow);

  // Attach CEGUI mouse handler, and set position
  m_guiMouseEventSubscriber.reset(new SubscriberRAIIWrapper(
      m_notificationsManager.getNotifier(MouseManager::NOTIFIER_NAME),
      m_name + "_guiHandler",
      std::bind(&WindowedRenderingApp::onMouseEvent, this,
                std::placeholders::_1, std::placeholders::_2),
      NOTIFICATIONPRIORITY_HIGHEST));
  const OIS::MouseState& mouseState = m_mouse->getMouseState();
  mouseState.width = width;
  mouseState.height = height;
  CEGUI::MouseCursor::setInitialMousePosition(
      CEGUI::Vector2f(mouseState.X.abs, mouseState.Y.abs));

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

bool WindowedRenderingApp::onMouseEvent(const std::string& name,
                                        const boost::any& parameters) {
  const MouseEventParams& params =
      boost::any_cast<std::reference_wrapper<const MouseEventParams>>(
          parameters);
  CEGUI::GUIContext& guiContext =
      CEGUI::System::getSingleton().getDefaultGUIContext();
  static bool fMousePressCaptured = false;

  if (name == MouseManager::MOVED) {
    guiContext.injectMousePosition(std::get<0>(params).state.X.abs,
                                   std::get<0>(params).state.Y.abs);
    // Always pass thru moved events
  } else if (name == MouseManager::PRESSED) {
    fMousePressCaptured = guiContext.injectMouseButtonDown(
        static_cast<CEGUI::MouseButton>(std::get<1>(params)));
    return fMousePressCaptured;
  } else if (name == MouseManager::RELEASED) {
    bool releaseHandled = guiContext.injectMouseButtonUp(
        static_cast<CEGUI::MouseButton>(std::get<1>(params)));
    // Handle mouse release only if mouse press is captured
    return releaseHandled & fMousePressCaptured;
  }
  return false;
}
