#ifndef _HEIGHT_FIELD_VISUALIZER_H_
#define _HEIGHT_FIELD_VISUALIZER_H_

#include <CGAL/Plane_3.h>

#include <OGRE/OgreEntity.h>
#include <OGRE/OgreSceneManager.h>
#include <OGRE/OgreSceneNode.h>

// Visualize a planar section of a scalar field as a height field.
template <class Field>
class HeightFieldVisualizer {
 public:
  HeightFieldVisualizer(
      const Field& inducedField,
      const HeightFieldVisualizationParams& visualizationParams,
      Ogre::SceneNode* parent)
      : m_inducedField(&inducedField),
        m_plane(plane),
        m_heightFieldSceneNode(parent->createChildSceneNode()) {
    addHeightFieldVisualizerToScene();
    recomputeVisualization();
  }

  void setPlane(const Kernel::Plane_3& plane) {
    m_plane = plane;
    recomputeVisualization();
  }

  void addHeightFieldVisualizerToScene() {
    UniformPlanarGrid planarGrid(m_visParams.plane(), m_visParams.x_res(),
                                 m_visParams.y_res(), m_visParams.x_extent(),
                                 m_visParams.y_extent());

    Ogre::Entity* heightFieldEntity =
        Framework::AppContext::getDynamicMeshManager().addMesh(
            planarGrid, m_heightFieldSceneNode);

    std::function<Kernel::FT(const Kernel::Point_2&)> samplingFunction =
        [ this, inducedFieldCRef =
                    std::cref(*m_inducedField) ](const Kernel::Point_2& point) {
      return inducedFieldCRef.get()(point) - m_value;
    };

    Finite_vertices_iterator iter =
        triangulation.finite_vertices_begin();
    for (; iter != triangulation.finite_vertices_end(); ++iter) {
      Kernel::FT sample = samplingFunction(iter->point());
      iter->info() = CGAL::Color(sample, sample, sample);
    }
  }

 private:
  Kernel::FT m_value;
  Ogre::SceneNode* m_heightFieldSceneNode;
  const Field* m_inducedField;
  HeightFieldVisualizationParams m_visParams;
};

#endif  //_HEIGHT_FIELD_VISUALIZER_H_