#ifndef _HEIGHT_FIELD_VISUALIZER_H_
#define _HEIGHT_FIELD_VISUALIZER_H_

#include <CGAL/Plane_3.h>

#include <OGRE/OgreEntity.h>
#include <OGRE/OgreManualObject.h>
#include <OGRE/OgreSceneManager.h>
#include <OGRE/OgreSceneNode.h>

#include <uniformPlanarGrid.h>

// TODO msati3: Move this to protobuf
struct HeightFieldVisualizationParams {
  HeightFieldVisualizationParams()
      : plane(Kernel::Point_3(0, 0, 0), Kernel::Point_3(1, 0, 0),
              Kernel::Point_3(0, 1, 0)),
        x_res(1000),
        y_res(1000),
        x_extent(10),
        y_extent(10) {}

  Kernel::Plane_3 plane;
  size_t x_res;
  size_t y_res;
  float x_extent;
  float y_extent;
};

// Visualize a planar section of a scalar field as a height field.
template <class Field>
class HeightFieldVisualizer {
 public:
  HeightFieldVisualizer(
      Ogre::SceneNode* parent,
      const HeightFieldVisualizationParams& visualizationParams =
          HeightFieldVisualizationParams())
      : m_visParams(visualizationParams),
        m_heightFieldSceneNode(parent->createChildSceneNode()),
        m_heightFieldObject(nullptr) {}

  void setPlane(const Kernel::Plane_3& plane) {
    m_visParams.plane = plane;
    recomputeVisualization();
  }

  void setField(const Field* inducedField) {
    m_inducedField = inducedField;
    recomputeVisualization();
  }

  void recomputeVisualization() {
    UniformPlanarGrid planarGrid(m_visParams.plane, m_visParams.x_res,
                                 m_visParams.y_res, m_visParams.x_extent,
                                 m_visParams.y_extent);

    std::function<Kernel::FT(const Kernel::Point_2&)> samplingFunction =
        [ this, inducedFieldCRef =
                    std::cref(*m_inducedField) ](const Kernel::Point_2& point) {
      return inducedFieldCRef.get()(point);
    };

    std::vector<std::tuple<Kernel::Point_2, float>> gridSamples;
    gridSamples.reserve(planarGrid.size());
    for (auto iter = planarGrid.begin(); iter != planarGrid.end(); ++iter) {
      Kernel::Point_2 pointPlanar = Kernel::Point_2((*iter)[0], (*iter)[1]);
      Kernel::FT sample = samplingFunction(pointPlanar);
      gridSamples.push_back(std::make_tuple(pointPlanar, sample));
    }

    if (m_heightFieldObject != nullptr) {
      m_heightFieldSceneNode->getCreator()->destroyManualObject(
          m_heightFieldObject);
    }

    m_heightFieldObject =
        m_heightFieldSceneNode->getCreator()->createManualObject("heightField");
    m_heightFieldObject->begin("Materials/PassThrough",
                               Ogre::RenderOperation::OT_POINT_LIST);

    auto minMaxEntries =
        std::minmax_element(gridSamples.begin(), gridSamples.end(),
                            [](const auto& first, const auto& second) {
                              return std::get<1>(first) < std::get<1>(second);
                            });
    auto minMaxValue = std::make_pair(std::get<1>(*minMaxEntries.first),
                                      std::get<1>(*minMaxEntries.second));

    float normalizingFactor = minMaxValue.second - minMaxValue.first;
    normalizingFactor = normalizingFactor == 0 ? 1 : normalizingFactor;

    for (const auto& gridSampleTuple : gridSamples) {
      m_heightFieldObject->position(
          std::get<0>(gridSampleTuple).x(), std::get<0>(gridSampleTuple).y(),
          std::get<1>(gridSampleTuple) - minMaxValue.first);
      float colorValue = (std::get<1>(gridSampleTuple) - minMaxValue.first) /
                         normalizingFactor;
      if ((int)(colorValue / 0.005) % 2 == 0) {
        m_heightFieldObject->colour(Ogre::ColourValue(0, colorValue, 0));
      } else {
        m_heightFieldObject->colour(Ogre::ColourValue(0, 0, 1 - colorValue));
      }
    }
    m_heightFieldObject->end();

    m_heightFieldSceneNode->attachObject(m_heightFieldObject);
  }

 private:
  Ogre::SceneNode* m_heightFieldSceneNode;
  const Field* m_inducedField;
  HeightFieldVisualizationParams m_visParams;
  Ogre::ManualObject* m_heightFieldObject;
};

#endif  //_HEIGHT_FIELD_VISUALIZER_H_
