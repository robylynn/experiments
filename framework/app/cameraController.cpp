#include "appContext.h"
#include "cameraController.h"
#include "inputManager.h"

namespace Context = Framework::AppContext;

CameraController::CameraController(const std::string& name, Ogre::Camera* cam)
    : mCamera(0),
      mTarget(0),
      mOrbiting(false),
      mZooming(false),
      mTopSpeed(150),
      mVelocity(Ogre::Vector3::ZERO),
      mGoingForward(false),
      mGoingBack(false),
      mGoingLeft(false),
      mGoingRight(false),
      mGoingUp(false),
      mGoingDown(false),
      mFastMove(false),
      mMouseEventSubscriber(
          Context::getNotificationsManager().getNotifier(
              MouseManager::NOTIFIER_NAME),
          name, std::bind(&CameraController::onMouseEvent, this,
                          std::placeholders::_1, std::placeholders::_2)),
      mKeyboardEventSubscriber(
          Context::getNotificationsManager().getNotifier(
              KeyboardManager::NOTIFIER_NAME),
          name, std::bind(&CameraController::onKeyboardEvent, this,
                          std::placeholders::_1, std::placeholders::_2)) {
  setCamera(cam);
  setStyle(CS_ORBIT);
}

/*-----------------------------------------------------------------------------
| Sets the target we will revolve around. Only applies for orbit style.
-----------------------------------------------------------------------------*/
void CameraController::setTarget(Ogre::SceneNode* target) {
  if (target != mTarget) {
    mTarget = target;
    if (target) {
      setYawPitchDist(Ogre::Degree(0), Ogre::Degree(15), 150);
      mCamera->setAutoTracking(true, mTarget);
    } else {
      mCamera->setAutoTracking(false);
    }
  }
}

/*-----------------------------------------------------------------------------
| Sets the spatial offset from the target. Only applies for orbit style.
-----------------------------------------------------------------------------*/
void CameraController::setYawPitchDist(Ogre::Radian yaw, Ogre::Radian pitch,
                                       Ogre::Real dist) {
  mCamera->setPosition(mTarget->_getDerivedPosition());
  mCamera->setOrientation(mTarget->_getDerivedOrientation());
  mCamera->yaw(yaw);
  mCamera->pitch(-pitch);
  mCamera->moveRelative(Ogre::Vector3(0, 0, dist));
}

/*-----------------------------------------------------------------------------
| Sets the movement style of our camera man.
-----------------------------------------------------------------------------*/
void CameraController::setStyle(CameraStyle style) {
  if (mStyle != CS_ORBIT && style == CS_ORBIT) {
    setTarget(mTarget ? mTarget
                      : mCamera->getSceneManager()->getRootSceneNode());
    mCamera->setFixedYawAxis(true);
    manualStop();
    setYawPitchDist(Ogre::Degree(0), Ogre::Degree(15), 150);

  } else if (mStyle != CS_FREELOOK && style == CS_FREELOOK) {
    mCamera->setAutoTracking(false);
    mCamera->setFixedYawAxis(true);
  } else if (mStyle != CS_MANUAL && style == CS_MANUAL) {
    mCamera->setAutoTracking(false);
    manualStop();
  }
  mStyle = style;
}

/*-----------------------------------------------------------------------------
| Manually stops the camera when in free-look mode.
-----------------------------------------------------------------------------*/
void CameraController::manualStop() {
  if (mStyle == CS_FREELOOK) {
    mGoingForward = false;
    mGoingBack = false;
    mGoingLeft = false;
    mGoingRight = false;
    mGoingUp = false;
    mGoingDown = false;
    mVelocity = Ogre::Vector3::ZERO;
  }
}

bool CameraController::frameRenderingQueued(const Ogre::FrameEvent& evt) {
  if (mStyle == CS_FREELOOK) {
    // build our acceleration vector based on keyboard input composite
    Ogre::Vector3 accel = Ogre::Vector3::ZERO;
    if (mGoingForward) accel += mCamera->getDirection();
    if (mGoingBack) accel -= mCamera->getDirection();
    if (mGoingRight) accel += mCamera->getRight();
    if (mGoingLeft) accel -= mCamera->getRight();
    if (mGoingUp) accel += mCamera->getUp();
    if (mGoingDown) accel -= mCamera->getUp();

    // if accelerating, try to reach top speed in a certain time
    Ogre::Real topSpeed = mFastMove ? mTopSpeed * 20 : mTopSpeed;
    if (accel.squaredLength() != 0) {
      accel.normalise();
      mVelocity += accel * topSpeed * evt.timeSinceLastFrame * 10;
    }
    // if not accelerating, try to stop in a certain time
    else
      mVelocity -= mVelocity * evt.timeSinceLastFrame * 10;

    Ogre::Real tooSmall = std::numeric_limits<Ogre::Real>::epsilon();

    // keep camera velocity below top speed and above epsilon
    if (mVelocity.squaredLength() > topSpeed * topSpeed) {
      mVelocity.normalise();
      mVelocity *= topSpeed;
    } else if (mVelocity.squaredLength() < tooSmall * tooSmall)
      mVelocity = Ogre::Vector3::ZERO;

    if (mVelocity != Ogre::Vector3::ZERO)
      mCamera->move(mVelocity * evt.timeSinceLastFrame);
  }

  return true;
}

/*-----------------------------------------------------------------------------
| Processes key presses for free-look style movement.
-----------------------------------------------------------------------------*/
void CameraController::injectKeyDown(const OIS::KeyEvent& evt) {
  if (mStyle == CS_FREELOOK) {
    if (evt.key == OIS::KC_W || evt.key == OIS::KC_UP)
      mGoingForward = true;
    else if (evt.key == OIS::KC_S || evt.key == OIS::KC_DOWN)
      mGoingBack = true;
    else if (evt.key == OIS::KC_A || evt.key == OIS::KC_LEFT)
      mGoingLeft = true;
    else if (evt.key == OIS::KC_D || evt.key == OIS::KC_RIGHT)
      mGoingRight = true;
    else if (evt.key == OIS::KC_PGUP)
      mGoingUp = true;
    else if (evt.key == OIS::KC_PGDOWN)
      mGoingDown = true;
    else if (evt.key == OIS::KC_LSHIFT)
      mFastMove = true;
  }
}

/*-----------------------------------------------------------------------------
| Processes key releases for free-look style movement.
-----------------------------------------------------------------------------*/
void CameraController::injectKeyUp(const OIS::KeyEvent& evt) {
  if (mStyle == CS_FREELOOK) {
    if (evt.key == OIS::KC_W || evt.key == OIS::KC_UP)
      mGoingForward = false;
    else if (evt.key == OIS::KC_S || evt.key == OIS::KC_DOWN)
      mGoingBack = false;
    else if (evt.key == OIS::KC_A || evt.key == OIS::KC_LEFT)
      mGoingLeft = false;
    else if (evt.key == OIS::KC_D || evt.key == OIS::KC_RIGHT)
      mGoingRight = false;
    else if (evt.key == OIS::KC_PGUP)
      mGoingUp = false;
    else if (evt.key == OIS::KC_PGDOWN)
      mGoingDown = false;
    else if (evt.key == OIS::KC_LSHIFT)
      mFastMove = false;
  }
}

/*-----------------------------------------------------------------------------
| Processes mouse movement differently for each style.
-----------------------------------------------------------------------------*/
void CameraController::injectMouseMove(const OIS::MouseEvent& evt) {
  if (mStyle == CS_ORBIT) {
    Ogre::Real dist =
        (mCamera->getPosition() - mTarget->_getDerivedPosition()).length();

    if (mOrbiting)  // yaw around the target, and pitch locally
    {
      mCamera->setPosition(mTarget->_getDerivedPosition());

      mCamera->yaw(Ogre::Degree(-evt.state.X.rel * 0.25f));
      mCamera->pitch(Ogre::Degree(-evt.state.Y.rel * 0.25f));

      mCamera->moveRelative(Ogre::Vector3(0, 0, dist));

      // don't let the camera go over the top or around the bottom of the
      // target
    } else if (mZooming)  // move the camera toward or away from the target
    {
      // the further the camera is, the faster it moves
      mCamera->moveRelative(
          Ogre::Vector3(0, 0, evt.state.Y.rel * 0.004f * dist));
    } else if (evt.state.Z.rel !=
               0)  // move the camera toward or away from the target
    {
      // the further the camera is, the faster it moves
      mCamera->moveRelative(
          Ogre::Vector3(0, 0, -evt.state.Z.rel * 0.0008f * dist));
    }
  } else if (mStyle == CS_FREELOOK) {
    mCamera->yaw(Ogre::Degree(-evt.state.X.rel * 0.15f));
    mCamera->pitch(Ogre::Degree(-evt.state.Y.rel * 0.15f));
  }
}

/*-----------------------------------------------------------------------------
| Processes mouse presses. Only applies for orbit style.
| Left button is for orbiting, and right button is for zooming.
-----------------------------------------------------------------------------*/
void CameraController::injectMouseDown(const OIS::MouseEvent& evt,
                                       OIS::MouseButtonID id) {
  if (mStyle == CS_ORBIT) {
    if (id == OIS::MB_Left)
      mOrbiting = true;
    else if (id == OIS::MB_Right)
      mZooming = true;
  }
}

/*-----------------------------------------------------------------------------
| Processes mouse releases. Only applies for orbit style.
| Left button is for orbiting, and right button is for zooming.
-----------------------------------------------------------------------------*/
void CameraController::injectMouseUp(const OIS::MouseEvent& evt,
                                     OIS::MouseButtonID id) {
  if (mStyle == CS_ORBIT) {
    if (id == OIS::MB_Left)
      mOrbiting = false;
    else if (id == OIS::MB_Right)
      mZooming = false;
  }
}

bool CameraController::onKeyboardEvent(const std::string& name,
                                       const boost::any& parameters) {
  const KeyboardEventParams& params =
      boost::any_cast<std::reference_wrapper<const KeyboardEventParams>>(
          parameters);
  if (name == KeyboardManager::PRESSED) {
    injectKeyDown(params);
  } else if (name == KeyboardManager::RELEASED) {
    injectKeyUp(params);
  }
  return true;
}

bool CameraController::onMouseEvent(const std::string& name,
                                    const boost::any& parameters) {
  const MouseEventParams& params =
      boost::any_cast<std::reference_wrapper<const MouseEventParams>>(
          parameters);
  if (name == MouseManager::MOVED) {
    injectMouseMove(std::get<0>(params));
  } else if (name == MouseManager::PRESSED) {
    injectMouseDown(std::get<0>(params), std::get<1>(params));
  } else if (name == MouseManager::RELEASED) {
    injectMouseUp(std::get<0>(params), std::get<1>(params));
  }
  return true;
}
