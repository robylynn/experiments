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

    std::function<Kernel::FT(const Kernel::Point_2&)> samplingFunction =
        [ this, inducedFieldCRef =
                    std::cref(*m_inducedField) ](const Kernel::Point_2& point) {
      return inducedFieldCRef.get()(point) - m_value;
    };

    std::vector<Kernel::Point_3> pointSamples;
    pointSamples.reserve(planarGrid.size());
    std::copy(planarGrid.begin(), planarGrid.end(),
              std::back_inserter(pointSamples));
    std::vector<CGAL::Color> colorSamples;
    colorSamples.reserve(pointSamples.size());

    auto colorIter = fieldValues.begin();
    for (auto iter = pointSamples.begin(); iter != pointSamples.end();
         ++iter, ++colorIter) {
      Kernel::FT sample = samplingFunction(iter->point());
      *colorIter = CGAL::Color(sample, sample, sample);
    }

    Ogre::Entity* heightFieldEntity =
        Framework::AppContext::getDynamicMeshManager().addMesh(
            planarGrid, m_heightFieldSceneNode,
            UniformPlanarGridInterpretationTag(),
            std::tuple<PositionAttribute_3, ColorAttribute>, pointSamples,
            fieldValues);
  }

 private:
  Kernel::FT m_value;
  Ogre::SceneNode* m_heightFieldSceneNode;
  const Field* m_inducedField;
  HeightFieldVisualizationParams m_visParams;
};

#endif  //_HEIGHT_FIELD_VISUALIZER_H_
