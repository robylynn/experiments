#ifndef _FRAMEWORK_APP_CONTEXT_H_
#define _FRAMEWORK_APP_CONTEXT_H_

#include "windowedRenderingApp.h"

namespace Framework {

// Provides access to app wide services
namespace AppContext {
using Context = WindowedRenderingApp;

static RenderingServicesManager& getRenderingServices() {
  return Context::getSingleton().getRenderingServicesManager();
}

static DynamicMeshManager& getDynamicMeshManager() {
  return getRenderingServices().getDynamicMeshManager();
}

static SelectionManager& getSelectionManager() {
  return getRenderingServices().getSelectionManager();
}

static NotificationsManager& getNotificationsManager() {
  return Context::getSingleton().getNotificationsManager();
}

}  // end namespace AppContext

}  // end namespace Framework

#endif  //_FRAMEWORK_APP_CONTEXT_H_
