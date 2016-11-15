#include <OGRE/OgreSceneManager.h>
#include <OGRE/OgreEntity.h>

#include <CGAL/Arr_linear_traits_2.h>
#include <CGAL/Arrangement_2.h>
#include <CGAL/Segment_2.h>
#include <CGAL/Sweep_line_2_algorithms.h>

#include <appContext.h>
#include <dynamicMeshManager.h>
#include <polyloop_2.h>

#include "averagingPolyloops_2View.h"

constexpr int NUM_LOOPS = 1;

namespace Context = Framework::AppContext;

AveragingPolyloops_2View::AveragingPolyloops_2View(Ogre::SceneNode* rootNode)
    : m_rootNode(rootNode),
      m_polyloopsRootNode(m_rootNode->createChildSceneNode()) {}

void AveragingPolyloops_2View::populateData() {
  std::vector<Polyloop_2> loops;
  for (int i = 0; i < NUM_LOOPS; ++i) {
    std::string loopName = "loop" + std::to_string(i) + "_2D";
    loops.push_back(Polyloop_2());
    buildPolyloopFromObj("data/" + loopName + ".obj", loops[loops.size() - 1]);
    // m_squaredDistField->addGeometry(loops[loops.size()-1]);

    Ogre::Entity* pEntity = Context::getDynamicMeshManager().addMesh(
        loops[loops.size() - 1], m_polyloopsRootNode);
    pEntity->setMaterialName("Materials/DefaultLines");
  }

  CGAL::Arrangement_2<CGAL::Arr_linear_traits_2<Kernel>> arrangement;

  std::list<CGAL::Arr_linear_traits_2<Kernel>::X_monotone_curve_2> lines;

  // Create edge segments that will be used for creating the arrangement
  for (auto iter = loops[0].begin(); iter != loops[0].end(); ++iter) {
    Kernel::Line_2 lineIn = loops[0].getSegment(prev(iter)).supporting_line();
    Kernel::Line_2 lineOut = loops[0].getSegment(iter).supporting_line();
    Kernel::Line_2 normalIn = lineIn.perpendicular(*iter);
    Kernel::Line_2 normalOut = lineOut.perpendicular(*iter);
    Kernel::Line_2 bisector = CGAL::bisector(lineIn, lineOut);

    lines.push_back(lineIn);
    lines.push_back(lineOut);
    lines.push_back(normalIn);
    lines.push_back(normalOut);
    lines.push_back(bisector);
  }

  std::list<decltype(lines)::value_type> subLines;
  CGAL::compute_subcurves(lines.begin(), lines.end(),
                          std::back_inserter(subLines));
  // CGAL::insert(arrangement, lines.begin(), lines.end());
}
