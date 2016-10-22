#include <map>

#include <glog/logging.h>

#include "prefabs.h"
#include "manualObjects.h"

namespace {
class PrefabFactory {
 public:
  static Ogre::ManualObject* getPrefab(const Prefab& prefab) {
    auto iter = m_prefabObjects.find(prefab);
    if (iter != m_prefabObjects.end()) {
      return iter->second;
    } else {
      Ogre::ManualObject* createdObject = nullptr;
      switch (prefab) {
        case Prefab::AXES:
          createdObject = createCoordinateAxes();
          break;
        case Prefab::PLANE:
          createdObject = createPlane();
          break;
        default:
          LOG(ERROR) << "Requested a prefab that is not present in the "
                        "prefab library";
      }
      m_prefabObjects[prefab] = createdObject;
      return createdObject;
    }
  }

 private:
  static std::map<Prefab, Ogre::ManualObject*> m_prefabObjects;
};

std::map<Prefab, Ogre::ManualObject*> PrefabFactory::m_prefabObjects;
}  // end anonymous namespace

Ogre::ManualObject* getPrefab(const Prefab& prefab) {
  return PrefabFactory::getPrefab(prefab);
}
