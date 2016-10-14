#include <iostream>
#include <memory>

#include <glog/logging.h>

#include <OGRE/Ogre.h>
#include <OGRE/OgreRectangle2D.h>
#include <OGRE/OgreConfigFile.h>

#include <CGAL/Point_2.h>
#include <CGAL/Line_2.h>
#include <CGAL/Ray_2.h>
#include <CGAL/Segment_2.h>
#include <CGAL/Polygon_2.h>
#include <CGAL/Polyline_simplification_2/simplify.h>

#include <cameraController.h>
#include <windowedRenderingApp.h>

#include <polyloop.h>
#include <polyloopGeometryProvider.h>
#include <positionOnlyBufferProvider.h>
#include <geometryRenderable.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

using Kernel1 = CGAL::Exact_predicates_exact_constructions_kernel;
namespace PS = CGAL::Polyline_simplification_2;
typedef CGAL::Polygon_2<Kernel1> Polygon_2;

int main() {
  PS::Stop_below_count_ratio_threshold stopThreshold(0.5);
  //PS::Stop_above_cost_threshold stopThreshold(0.1);
  PS::Squared_distance_cost cost;
  Polygon_2 polygon;
  std::cin >> polygon;

  Polygon_2 simplified = PS::simplify(polygon, cost, stopThreshold);
  std::cout << polygon.size() << " " << simplified.size() << std::endl;
  std::cout << polygon << std::endl;
  std::cout << simplified << std::endl;
  return 0;
}

/*template <typename T>
using PositionRenderable = GeometryRenderable<PositionOnlyBufferProvider<T>>;

bool initScene(const std::string& windowName, const std::string& sceneName) {
  Ogre::Root* root = Ogre::Root::getSingletonPtr();
  Ogre::SceneManager* sceneManager =
      root->createSceneManager(Ogre::ST_GENERIC, sceneName);
  sceneManager->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));

  Ogre::Camera* mainCamera = sceneManager->createCamera("PrimaryCamera");
  Ogre::Viewport* viewport =
      root->getRenderTarget(windowName)->addViewport(mainCamera);
  viewport->setBackgroundColour(Ogre::ColourValue(1, 1, 1));

  mainCamera->setAspectRatio((float)viewport->getActualWidth() /
                             viewport->getActualHeight());
  mainCamera->setPosition(Ogre::Vector3(0, 0, 200));
  mainCamera->lookAt(Ogre::Vector3(0, 0, 0));
  mainCamera->setNearClipDistance(5);

  CameraController* cameraController =
      new CameraController("mainCamera", mainCamera);
}

template <typename T>
using RenderBufferProvider = PositionOnlyBufferProvider<T>;
template <typename T>
using Renderable = GeometryRenderable<RenderBufferProvider<T>>;

int main(int argc, char* argv[]) {
  google::InitGoogleLogging(argv[0]);

  WindowedRenderingApp app("Smoothing");

  Polyloop<CGAL::Point_3<Kernel>> p;
  p.addPoint(CGAL::Point_3<Kernel>(0, 0, 0));
  p.addPoint(CGAL::Point_3<Kernel>(100, 100, -100));
  p.addPoint(CGAL::Point_3<Kernel>(100, 0, -100));

  if (app.init(800, 800)) {
    initScene(app.getWindowName(), "PrimaryScene");

    Ogre::Root* root = Ogre::Root::getSingletonPtr();
    Ogre::SceneManager* sceneManager = root->getSceneManager("PrimaryScene");

    using LoopGeometryProvider =
        PolyloopGeometryProvider<CGAL::Point_3<Kernel>>;

    LoopGeometryProvider loopGeometryProvider(p);
    auto loopRenderable = new Renderable<LoopGeometryProvider>();
    loopRenderable->setVertexData(
        RenderBufferProvider<LoopGeometryProvider>(loopGeometryProvider));

    sceneManager->getRootSceneNode()->createChildSceneNode()->attachObject(
        loopRenderable);

    std::function<Kernel::FT(const Kernel::Point_3&)> samplingFunction =
        [](const Kernel::Point_3& point) {
          return CGAL::squared_distance(point, Kernel::Point_3(0, 0, 0)) - 100;
        };

    using TMeshRepresentation = typename LevelSetMeshBuilder<>::Representation;
    using TMeshGeometryProvider =
        TriangleMeshGeometryProvider<TMeshRepresentation>;

    LevelSetMeshBuilder<> meshBuilder;
    TMeshRepresentation meshRep = meshBuilder.buildMesh(
        samplingFunction, Kernel::Sphere_3(CGAL::ORIGIN, 1000), 1);
    TMeshGeometryProvider meshGeometryProvider(meshRep);
    auto meshRenderable = new Renderable<TMeshGeometryProvider>();
    meshRenderable->setVertexData(
        RenderBufferProvider<TMeshGeometryProvider>(meshGeometryProvider));

    sceneManager->getRootSceneNode()->createChildSceneNode()->attachObject(
        meshRenderable);

    app.startEventLoop();
  }
  return 0;
}
*/
