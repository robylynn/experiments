#ifndef _NOTIFIER_H_
#define _NOTIFIER_H_

#include <glog/logging.h>

#include <memory>
#include <mutex>

#include <boost/noncopyable.hpp>
#include <boost/any.hpp>

enum NotificationPriority {
  NOTIFICATIONPRIORITY_HIGHEST = 1,
  NOTIFICATIONPRIORITY_DEFAULT = 100,
};

// Thread safe, generic notification registration object. Caller must call
// unregisterForNotifications before dying. Notification is synchronous.
// A unique channel of communication between a subscriber and notifier is
// called an endpoint. Multiple named notifications may be generated at the
// endpoint, with the same set of parameters, distinguished by the notification
// name. Callbacks are called in order of priority, and, if they handle a
// particular notification by returning a bool, this is not sent to lower
// priority subscribers. Lower numbers have higher priority.
class Notifier {
 public:
  void registerForNotifications(
      const std::string& endpointName,
      const std::function<bool(const std::string&, const boost::any&)>&
          callback,
      size_t priority = NOTIFICATIONPRIORITY_DEFAULT) {
    std::lock_guard<std::mutex> lock(m_callbacksMutex);
    m_callbacksMap.insert(
        std::make_pair(std::make_pair(priority, endpointName), callback));
    m_endpointPriorityMap.insert(std::make_pair(endpointName, priority));
  }

  void unregisterForNotifications(const std::string& endpointName) {
    std::lock_guard<std::mutex> lock(m_callbacksMutex);
    size_t priority = m_endpointPriorityMap[endpointName];
    m_callbacksMap.erase(
        std::make_pair(m_endpointPriorityMap[endpointName], endpointName));
    m_endpointPriorityMap.erase(endpointName);
    DLOG_IF(ERROR, m_callbacksMap.find(std::make_tuple(
                       priority, endpointName)) != m_callbacksMap.end())
        << "Multiple endpoints with same name found in callback map";
  }

  void notify(const std::string& notificationName,
              const boost::any& parameters) {
    std::lock_guard<std::mutex> lock(m_callbacksMutex);
    bool fHandled = false;
    size_t handledPriority = 0;
    for (auto const& endpointCallbackPair : m_callbacksMap) {
      // Notifiy all callbacks, untile we get a handled response. Then, notify
      // all callbacks and same priority, and break out of loop.
      size_t callBackPriority = std::get<0>(std::get<0>(endpointCallbackPair));
      if (fHandled && handledPriority < callBackPriority) break;
      bool fHandle =
          (std::get<1>(endpointCallbackPair))(notificationName, parameters);
      if (!fHandled) fHandled = fHandle;
      handledPriority = fHandle ? callBackPriority : handledPriority;
    }
  }

 private:
  // TODO msati3: Switch to shared_mutex in C++17
  std::mutex m_callbacksMutex;
  std::multimap<std::tuple<size_t, std::string>,
                std::function<bool(const std::string& notificationName,
                                   const boost::any&)>> m_callbacksMap;
  std::map<std::string, size_t> m_endpointPriorityMap;
};

// RAII-style notification registration wrapper. Automatically unregisters upon
// destruction if the notifier is alive.
class SubscriberRAIIWrapper : boost::noncopyable {
 public:
  SubscriberRAIIWrapper(const std::shared_ptr<Notifier>& notifier,
                        const std::string& endpointName,
                        const std::function<bool(const std::string&,
                                                 const boost::any&)>& callback,
                        size_t priority = NOTIFICATIONPRIORITY_DEFAULT)
      : m_notifier(notifier), m_endpointName(endpointName) {
    notifier->registerForNotifications(m_endpointName, callback, priority);
  }

  ~SubscriberRAIIWrapper() {
    if (auto strong_notifier = m_notifier.lock()) {
      strong_notifier->unregisterForNotifications(m_endpointName);
    }
  }

 private:
  const std::string m_endpointName;
  std::weak_ptr<Notifier> m_notifier;
};

// Manages notifiers in the sytem. Allows query of registered notifiers.
// Sharedly owns notifier instances unless unregistered.
class NotificationsManager {
 public:
  void registerNotifier(const std::string& name,
                        std::shared_ptr<Notifier> notifier) {
    std::lock_guard<std::mutex> lock(m_notifiersMutex);
    m_notifiersMap.insert(std::make_pair(name, notifier));
  }

  void unregisterNotifier(const std::string& name) {
    std::lock_guard<std::mutex> lock(m_notifiersMutex);
    m_notifiersMap.erase(name);
    DLOG_IF(ERROR, m_notifiersMap.find(name) != m_notifiersMap.end())
        << "Multiple endpoints with same name found in notifiers map";
  }

  const std::shared_ptr<Notifier>& getNotifier(const std::string& name) {
    std::lock_guard<std::mutex> lock(m_notifiersMutex);
    auto notifierIter = m_notifiersMap.find(name);
    DLOG_IF(ERROR, notifierIter == m_notifiersMap.end())
        << "The following notifier requested was not registered " << name;
    return notifierIter->second;
  }

 private:
  std::mutex m_notifiersMutex;
  std::map<std::string, std::shared_ptr<Notifier>> m_notifiersMap;
};

#endif  //_NOTIFIER_H_
