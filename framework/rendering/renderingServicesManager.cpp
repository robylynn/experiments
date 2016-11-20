#include "renderingServicesManager.h"
#include "dynamicMeshManager.h"
#include "selection/selectionManager.h"

RenderingServicesManager::RenderingServicesManager()
    : m_selectionManager(new SelectionManager()),
      m_dynamicMeshManager(new DynamicMeshManager()) {}

RenderingServicesManager::~RenderingServicesManager() {}

SelectionManager& RenderingServicesManager::getSelectionManager() {
  assert(m_selectionManager != nullptr);
  return *m_selectionManager;
}
const SelectionManager& RenderingServicesManager::getSelectionManager() const {
  assert(m_selectionManager != nullptr);
  return *m_selectionManager;
}

DynamicMeshManager& RenderingServicesManager::getDynamicMeshManager() {
  assert(m_dynamicMeshManager != nullptr);
  return *m_dynamicMeshManager;
}
