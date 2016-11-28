#ifndef _FIELD_LEVEL_SET_VISUALIZER_H_
#define _FIELD_LEVEL_SET_VISUALIZER_H_

#include <OGRE/OgreSceneNode.h>
#include <OGRE/OgreSceneManager.h>
#include <OGRE/OgreEntity.h>

#include <appContext.h>
#include <geometryTypes.h>
#include <dynamicMeshManager.h>

#include "levelSetMeshBuilder.h"

// View class that allows for rendering level sets of field.
template <class Field>
class LevelSetMeshVisualizer {
  static constexpr Kernel::FT MAX_LEVEL = 20;
  static constexpr Kernel::FT MIN_LEVEL = 1;

 public:
  LevelSetMeshVisualizer(Ogre::SceneNode* parent) : m_inducedField(nullptr) {
    m_levelSetSceneNode = parent->createChildSceneNode();
    setNormalizedLevel(0.1);
  }

  void setNormalizedLevel(Kernel::FT value) {
    m_value = (MAX_LEVEL - MIN_LEVEL) * value + MIN_LEVEL;
    addLevelSetMeshToScene();
  }

  void setField(const Field* inducedField) {
    m_inducedField = inducedField;
    clearLevelSetMeshes();
    addLevelSetMeshToScene();
  }

  void clearLevelSetMeshes() {
    for (auto levelSetMesh : m_levelSetMeshes) {
      m_levelSetSceneNode->getCreator()->destroyEntity(levelSetMesh);
    }
    m_levelSetMeshes.clear();
  }

  void addLevelSetMeshToScene() {
    if (!m_inducedField) return;

    std::function<Kernel::FT(const Kernel::Point_3&)> samplingFunction =
        [ this, inducedFieldCRef = std::cref(*m_inducedField) ](
            const Kernel::Point_3& point) {
      return inducedFieldCRef.get()(point) - m_value;
    };

    LevelSetMeshBuilder<> meshBuilder;
    CGAL::Polyhedron_3<Kernel> meshRep;
    meshBuilder.buildMesh(samplingFunction, Kernel::Sphere_3(CGAL::ORIGIN, 100),
                          1, meshRep);

    Ogre::Entity* levelSetMeshEntity =
        Framework::AppContext::getDynamicMeshManager().addMesh(
            meshRep, m_levelSetSceneNode);
    levelSetMeshEntity->setMaterialName(
        "Materials/DefaultTransparentTriangles");

    m_levelSetMeshes.push_back(levelSetMeshEntity);
  }

 private:
  Kernel::FT m_value;
  Ogre::SceneNode* m_levelSetSceneNode;
  std::vector<Ogre::Entity*> m_levelSetMeshes;
  const Field* m_inducedField;
};

#endif  //_FIELD_LEVEL_SET_VISUALIZER_H_
