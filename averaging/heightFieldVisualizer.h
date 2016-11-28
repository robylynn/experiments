#ifndef _HEIGHT_FIELD_VISUALIZER_H_
#define _HEIGHT_FIELD_VISUALIZER_H_

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
};

#endif //_HEIGHT_FIELD_VISUALIZER_H_
