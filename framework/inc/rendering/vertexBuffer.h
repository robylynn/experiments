#ifndef _FRAMEWORK_RENDERING_VERTEX_BUFFER_H_
#define _FRAMEWORK_RENDERING_VERTEX_BUFFER_H_

template <typename RenderPolicy>
void createVertexData(Ogre::VertexData** vertexData,
                      const RenderPolicy& renderPolicy) {
  *vertexData = OGRE_NEW Ogre::VertexData();
  (*vertexData)->vertexStart = renderPolicy.vertexStart;
  (*vertexData)->vertexCount = renderPolicy.vertexHint;

  // Create vertex elements (attributes) for the types required
  for (auto vertexElement : renderPolicy.vertexElements()) {
    boost::apply_visitor(CreateVertexElementVisitor(*vertexData),
                         vertexElement);
  }
}

template <typename RenderPolicy, typename VertexElementDataProvider>
void populateVertexElementData(Ogre::VertexData* vertexData,
                               const RenderPolicy& renderPolicy,
                               const VertexElementDataProvider& provider) {
  for (auto vertexElement : renderPolicy.vertexElements()) {
    boost::apply_visitor(
        PopulateVertexElementDataVisitor<VertexElementDataProvider>(vertexData,
                                                                    provider),
        vertexElement);
  }
}

#endif //_FRAMEWORK_RENDERING_VERTEXBUFFER_H_
