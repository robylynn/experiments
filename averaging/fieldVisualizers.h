#ifndef _FIELD_VISUALIZERS_H_
#define _FIELD_VISUALIZERS_H_

#include <OGRE/OgreSceneNode.h>
#include <OGRE/OgreSceneManager.h>
#include <OGRE/OgreEntity.h>

#include <appContext.h>
#include <geometryTypes.h>
#include <dynamicMeshManager.h>
#include <circleSampler.h>

#include "gradientComputer.h"
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

// Visualizes the field in the neighborhood of a sequence of vertices, by
// coloring a planar circle, with plane determined by the gradient of the field.
// This allows for the determination of principal directions of the iso-surface
// created locally by the field at each vertex.
template <class Field>
class VertexNeighborhoodFieldVisualizer {
 public:
  VertexNeighborhoodFieldVisualizer(Ogre::SceneNode* parent)
      : m_rootSceneNode(parent->createChildSceneNode()) {}

  template <typename GeomRepIter>
  void visualizeFieldAroundSamples(const Field& field, GeomRepIter begin,
                                   GeomRepIter end) {
    NaiveGradientEstimator estimator(0.05);
    GradientComputer<Field> gradientComputer(field, estimator);

    // Populate a vector of point, color values that will be used to provide
    // geometry for rendering by a disc geometry provider.
    std::vector<std::pair<Kernel::Point_3, Ogre::ColourValue>>
        circleDataProvider;

    for (auto iter = begin; iter != end; ++iter) {
      Kernel::Vector_3 gradient = gradientComputer(*iter);

      Kernel::FT centerValue = field(*iter);
      circleDataProvider.push_back(*iter, centerValue);

      CircleSampler circleSampler(*iter, 0.05,
                                  Kernel::Plane_3(*iter, gradient));
      for (const auto& point : circleSampler) {
        Kernel::FT fieldValue = field(point);
        circleDataProvider.push_back(point, fieldValue);
      }
    }
  }

 private:
  Ogre::SceneNode* m_rootSceneNode;
};

/*
template <class Field>
class PlanarDistanceFieldVisualizer {
 public:
  PlanarDistanceFieldVisualizer(const Field& inducedField,
                                Ogre::SceneNode* polyloopsSceneNode)
      : m_inducedField(&inducedField),
        m_polyloopsSceneNode(polyloopsSceneNode),
        m_distanceFieldSceneNode(
            polyloopsSceneNode->getParentSceneNode()->createChildSceneNode()) {}

  void addToScene() {
    // Customize some materials. TODO msati3: Add a node listener to the
    // polyloops scene node here.
    Ogre::AxisAlignedBox loopBounds = m_polyloopsSceneNode->_getWorldAABB();
    UniformPlanarGrid planarGrid(Kernel::Plane_3(0, 0, 1, 0), 10, 10, 6, 6);
    typedef CGAL::Projection_traits_xy_3<Kernel> GeometryTraits;
    typedef CGAL::Delaunay_triangulation_2<
        GeometryTraits,
        CGAL::Triangulation_data_structure_2<
            CGAL::Triangulation_vertex_base_with_info_2<CGAL::Color, Kernel>,
            CGAL::Triangulation_face_base_2<Kernel>>> Triangulation;
    Triangulation triangulation(planarGrid.begin(), planarGrid.end());

    using TMeshGeometryProvider = TriangleMeshGeometryProvider<Triangulation>;
    TMeshGeometryProvider meshGeometryProvider(triangulation);
    /*auto gridMeshRenderable =
        new Meshable<TMeshGeometryProvider>("scalarFieldHeatMap");
    gridMeshRenderable->setVertexBufferData(
        RenderBufferProvider<TMeshGeometryProvider>(meshGeometryProvider));

    Ogre::Root* root = Ogre::Root::getSingletonPtr();
    Ogre::SceneManager* sceneManager = root->getSceneManager("PrimaryScene");
    Ogre::Entity* gridEntity =
        sceneManager->createEntity("heatMap", "scalarFieldHeatMap");
    gridEntity->setMaterialName("Materials/DefaultTriangles");

    std::function<Kernel::FT(const Kernel::Point_2&)> samplingFunction =
        [ this, inducedFieldCRef = std::cref(*m_inducedField) ](
            const Kernel::Point_2& point) {
      return inducedFieldCRef.get()(point) - m_value;
    };

    Triangulation::Finite_vertices_iterator iter =
        triangulation.finite_vertices_begin();
    for (; iter != triangulation.finite_vertices_end(); ++iter) {
      Kernel::FT sample = samplingFunction(iter->point());
      iter->info() = CGAL::Color(sample, sample, sample);
    }

    // Remove all children from scene node, and re-populate with new level-set
    m_distanceFieldSceneNode->removeAndDestroyAllChildren();
    m_distanceFieldSceneNode->attachObject(gridEntity);
    m_distanceFieldSceneNode->showBoundingBox(true);
  }

 private:
  Kernel::FT m_value;
  Ogre::SceneNode* m_polyloopsSceneNode;
  Ogre::SceneNode* m_distanceFieldSceneNode;
  const Field* m_inducedField;
};*/

#endif  //_FIELD_VISUALIZERS_H_
