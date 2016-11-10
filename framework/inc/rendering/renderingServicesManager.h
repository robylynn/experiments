#ifndef _FRAMEWORK_RENDERING_RENDERING_SERVICES_MANAGER_H_
#define _FRAMEWORK_RENDERING_RENDERING_SERVICES_MANAGER_H_

#include <memory>

class DynamicMeshManager;
class SelectionManager;

class RenderingServicesManager {
 public:
  RenderingServicesManager();
  ~RenderingServicesManager();
  DynamicMeshManager& getDynamicMeshManager();
  SelectionManager& getSelectionManager();
  const SelectionManager& getSelectionManager() const;

 private:
  std::unique_ptr<DynamicMeshManager> m_dynamicMeshManager;
  std::unique_ptr<SelectionManager> m_selectionManager;
};

#endif  //_FRAMEWORK_RENDERING_RENDERING_SERVICES_MANAGER_H_
