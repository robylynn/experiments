#ifndef _NOTIFIER_H_
#define _NOTIFIER_H_

#include <memory>

#include <boost/noncopyable.hpp>
#include <boost/any.hpp>

#include <mutex>

class NotificationsManager;
NotificationsManager& getAppWideNotificationsManager();

// Thread safe, generic notification registration object. Caller must call
// unregisterForNotifications before dying. Notification is synchronous.
// A unique channel of communication between a subscriber and notifier is
// called an endpoint. Multiple named notifications may be generated at the
// endpoint, with the same set of parameters, distinguished by the notification
// name.
class Notifier {
 public:
  void registerForNotifications(
      const std::string& endpointName,
      const std::function<void(const std::string&, const boost::any&)>&
          callback) {
    std::lock_guard<std::mutex> lock(m_callbacksMutex);
    m_callbacksMap.insert(std::make_pair(endpointName, callback));
  }

  void unregisterForNotifications(const std::string& endpointName) {
    std::lock_guard<std::mutex> lock(m_callbacksMutex);
    m_callbacksMap.erase(endpointName);
    DLOG_IF(ERROR, m_callbacksMap.find(endpointName) != m_callbacksMap.end())
        << "Multiple endpoints with same name found in callback map";
  }

  void notify(const std::string& notificationName, const boost::any& parameters) {
    std::lock_guard<std::mutex> lock(m_callbacksMutex);
    for (auto const& endpointCallbackPair : m_callbacksMap) {
      (std::get<1>(endpointCallbackPair))(notificationName, parameters);
    }
  }

 private:
  // TODO msati3: Switch to shared_mutex in C++17
  std::mutex m_callbacksMutex;
  std::map<std::string, std::function<void(const std::string& notificationName,
                                           const boost::any&)>> m_callbacksMap;
};

// RAII-style notification registration wrapper. Automatically unregisters upon
// destruction if the notifier is alive.
class SubscriberRAIIWrapper : boost::noncopyable {
 public:
  SubscriberRAIIWrapper(const std::shared_ptr<Notifier>& notifier,
                        const std::string& endpointName,
                        const std::function<void(const std::string&,
                                                 const boost::any&)>& callback)
      : m_notifier(notifier), m_endpointName(endpointName) {
    notifier->registerForNotifications(m_endpointName, callback);
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
