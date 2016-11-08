#include <fstream>
#include <iostream>
#include <memory>

#include <glog/logging.h>
#include <gflags/gflags.h>

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

#include <psimpl.h>

#include <OGRE/Ogre.h>
#include <OGRE/OgreRectangle2D.h>
#include <OGRE/OgreConfigFile.h>

#include <cameraController.h>
#include <windowedRenderingApp.h>

#include <polyloop_3.h>
#include <polyline.h>
#include <polyloopGeometryProvider.h>
#include <defaultRenderables.h>
#include <defaultBufferProviders.h>
#include <prefabs.h>

DEFINE_string(
    file_basename,
    "/home/mukul/development/experiments/approximation/data/simplification/"
    "refined/curves",
    "Name of the file from which the polyloops to be visualized will be "
    "loaded");
DEFINE_string(file_indexes, "0-3",
              "Interval format indexes for polyloops to be visualized");
DEFINE_string(file_ext, ".txt", "Extension of the polyloop files");
DEFINE_string(
    output_dir,
    "/home/mukul/development/experiments/approximation/data/simplification/"
    "simplified/curves",
    "Name of the directory where simplified polyloops will be written");

bool initScene(const std::string& windowName, const std::string& sceneName) {
  Ogre::Root* root = Ogre::Root::getSingletonPtr();
  Ogre::SceneManager* sceneManager =
      root->createSceneManager(Ogre::ST_GENERIC, sceneName);
  sceneManager->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));

  Ogre::Camera* mainCamera = sceneManager->createCamera("PrimaryCamera");
  Ogre::Viewport* viewport =
      root->getRenderTarget(windowName)->addViewport(mainCamera);
  viewport->setBackgroundColour(Ogre::ColourValue(0.5, 0.5, 0.5));

  mainCamera->setNearClipDistance(0.5);
}

class RenderFrameCallbackHandler {
 public:
  RenderFrameCallbackHandler(const std::string& centeredNodeName)
      : m_centeredNodeName(centeredNodeName) {}

  void frameCallback() {
    static int counter = 0;

    if (counter < 3) {
      counter++;
    }

    if (counter == 2) {
      // One time delayed init
      // Switch camera to center of polyloop
      Ogre::Root* root = Ogre::Root::getSingletonPtr();
      Ogre::SceneManager* sceneManager = root->getSceneManager("PrimaryScene");
      Ogre::Camera* mainCamera = sceneManager->getCamera("PrimaryCamera");

      Ogre::SceneNode* loopNode = static_cast<Ogre::SceneNode*>(
          sceneManager->getRootSceneNode()->getChild(m_centeredNodeName));
      Ogre::SceneNode* loopCenterNode = loopNode->createChildSceneNode();
      loopCenterNode->setPosition(loopNode->_getWorldAABB().getCenter());

      CameraController* cameraController =
          new CameraController("PrimaryCameraController", mainCamera);
      cameraController->setTarget(loopCenterNode);
    }
  }

 private:
  std::string m_centeredNodeName;
};

int main(int argc, char* argv[]) {
  google::InitGoogleLogging(argv[0]);
  google::ParseCommandLineFlags(&argc, &argv, true);

  WindowedRenderingApp app("Smoothing");
  using Polyline_3 = Polyline<Kernel::Point_3>;

  if (app.init(1200, 900)) {
    initScene(app.getWindowName(), "PrimaryScene");

    Ogre::Root* root = Ogre::Root::getSingletonPtr();
    Ogre::SceneManager* sceneManager = root->getSceneManager("PrimaryScene");
    Ogre::Camera* mainCamera = sceneManager->getCamera("PrimaryCamera");
    app.getSelectionManager().setCamera(*mainCamera);

    std::vector<std::string> intervals;
    std::string indexString = std::string(FLAGS_file_indexes);
    boost::split(intervals, indexString, boost::is_any_of("-"));
    LOG_IF(ERROR, intervals.size() > 2 || intervals.size() < 1)
        << "The interval string should specify an interval or a single "
           "element";
    if (intervals.size() == 1) {
      intervals.push_back(intervals[0]);
    }
    size_t indexes[2]{boost::lexical_cast<size_t>(intervals[0]),
                      boost::lexical_cast<size_t>(intervals[1])};
    RenderFrameCallbackHandler callbackHandler("loopNode" + intervals[0]);

    for (int index = indexes[0]; index <= indexes[1]; ++index) {
      std::string strIndex = std::to_string(index);
      Polyline_3 p;
      if (!buildPolylineFromVertexList(
              FLAGS_file_basename + strIndex + FLAGS_file_ext, p)) {
        return -1;
      }

      Polyline_3 simplified;

      // Unroll data to flat form for use with psimpl's douglas peucker
      // algorithm
      using GeometryProvider =
          PolylineGeometryProvider<Polyline_3, PolylinePointPolicy>;
      using GeometryAdaptor =
          SingleElementProviderAdaptor<GeometryProvider, PositionVertexElement>;
      VertexElementBufferProvider<GeometryAdaptor, PositionVertexElement>
          flatIterProvider{GeometryProvider(p)};
      std::vector<float> simplifiedFlat;

      // Simplify polyline with tolerance.
      psimpl::simplify_douglas_peucker<3>(flatIterProvider.begin(),
                                          flatIterProvider.end(), 0.0015,
                                          std::back_inserter(simplifiedFlat));

      // Create simplified polyline from unrolled points.
      for (
          auto iter = utils::tuple_iterator<
              decltype(simplifiedFlat.begin()), 3,
              Kernel::Point_3>::begin(simplifiedFlat.begin(),
                                      simplifiedFlat.end());
          iter !=
              utils::tuple_iterator<decltype(simplifiedFlat.end()), 3,
                                    Kernel::Point_3>::end(simplifiedFlat.end());
          ++iter) {
        simplified.addPoint(*iter);
      }

      make_mesh_renderable(p, "loop" + strIndex);
      Ogre::Entity* loopEntity = sceneManager->createEntity("loop" + strIndex);
      Ogre::SceneNode* loopNode =
          sceneManager->getRootSceneNode()->createChildSceneNode("loopNode" +
                                                                 strIndex);
      loopNode->attachObject(loopEntity);

      make_mesh_renderable(simplified, "loopSimplified" + strIndex);
      Ogre::Entity* simplifiedLoopEntity =
          sceneManager->createEntity("loopSimplified" + strIndex);
      simplifiedLoopEntity->setMaterialName("Materials/SimplifiedLines");
      Ogre::SceneNode* simplifiedLoopNode =
          sceneManager->getRootSceneNode()->createChildSceneNode(
              "loopNodeSimplified" + strIndex);
      simplifiedLoopNode->attachObject(simplifiedLoopEntity);

      // Also, write the simplified loops to file
      std::string outputFilePath =
          FLAGS_output_dir + "/simplified" + strIndex + FLAGS_file_ext;
      LOG(ERROR) << "Writing simplified file to " << outputFilePath
                 << " with simplification ratio "
                 << simplified.size() / (float)p.size();
      std::ofstream ofile(outputFilePath);
      for (const auto& point : simplified) {
        ofile << point << "\n";
      }
    }

    Ogre::SceneNode* axesNode =
        sceneManager->getRootSceneNode()->createChildSceneNode();
    axesNode->attachObject(getPrefab(Prefab::AXES));
    axesNode->setScale(10, 10, 10);

    std::function<void(void)> callBackFn =
        std::bind(&RenderFrameCallbackHandler::frameCallback, &callbackHandler);
    app.startEventLoop(&callBackFn);
  }
  return 0;
}
