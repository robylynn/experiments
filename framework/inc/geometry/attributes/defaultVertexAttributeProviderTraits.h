#ifndef _FRAMEWORK_GEOMETRY_DEFAULT_VERTEX_ATTRIBUTE_PROVIDER_TRAITS_H_
#define _FRAMEWORK_GEOMETRY_DEFAULT_VERTEX_ATTRIBUTE_PROVIDER_TRAITS_H_

#include <CGAL/Polyhedron_3.h>

#include "polyline_3.h"
#include "polyloop_2.h"
#include "polyloop_3.h"
#include "simplicialAdaptors/polyloopSimplicialAdaptor.h"
#include "simplicialAdaptors/polylineSimplicialAdaptor.h"
#include "simplicialAdaptors/triangleMeshSimplicialAdaptor.h"

#include "attributes/vertexAttributeProviderTraits.h"

// Given a geometry representation, that we wish to visualize,  we need to
// communicate how it is to be rendered to the rendering system. Sometimes, we
// would just have a vector of points, and wish it to be interpreted as a
// collection of points samples to be interpreted as a Polyloop_3.
// Sometimes, we would have a Polyloop_3 object, and would want to send it
// to the rendering engine as well -- the desire is to make all of these work
// seamlessless. The solution here is that each GeometryRep will advertize its
// "GeometryProvider" - for most GeometryRep's this will be a simplicial
// adaptor that is capable of providing a simplicial decomposition to vertices
// of the geometry. The rendering engine will then transmit this information to
// the GPU, asking for appropriate primitives (TRIANGLE_FANS, TRIANGLE_LISTS,
// etc) that will map the supplied vertex stream to rendered images.

// A simplicial interpretation tag allows data to be understood in a
// particular simplicial type, and an appropriate simplicial adaptor used,
// as a provider. This is particularly useful for sending across a flat list of
// geometry in a container, and automatically picking a geometry interpretor
// that will understand it as being ordered in accordance with a particular
// GeometryRepresentation. This allows for interpreting for example, a vector
// of points as vectors oriented from the origin to the point; understanding a
// list of points to denote a disc geometry, consisting of first the center,
// and then subsequent vertices in order around the disc, etc.
template <typename GeometryInterpretationTag, typename T>
struct VertexAttributeProviderTraits<
    std::tuple<GeometryInterpretationTag, std::vector<T>>>,
    typename VertexAttributeFromType<T>::type >
        : public SimpleVertexAttributeProviderTraits<
              PolylineSimplicialAdaptor<GeometryInterpretationTag>> {
  using provider_type =
      PolylineSimplicialAdaptor<GeometryInterpretationTag>::type;
  using const_iterator =
      typename PolylineSimplicialAdaptor<GeometryPolyline_3>::const_iterator;
  using storage_strategy = AttributeProviderStorageStrategy<
      PolylineSimplicialAdaptor<Polyline<PointType>>>;
};

// In case the user passes a simple vector, the best guess is to interpret it
// as a list of ordered elements, and use a PolylineSimplicialAdaptor.
template <typename T>
struct VertexAttributeProviderTraits<
    std::vector<T>, typename VertexAttributeFromType<T>::type> {
  using provider_type = PolylineSimplicialAdaptor<std::vector<T>>;
  using const_iterator = typename provider_type::const_iterator;
  using storage_strategy = AttributeProviderStorageStrategy<provider_type>;
};

template <typename Kernel, typename VertexAttribute>
struct VertexAttributeProviderTraits<CGAL::Polyhedron_3<Kernel>,
                                     VertexAttribute> {
 private:
  using MeshType = CGAL::Polyhedron_3<Kernel>;

 public:
  using provider_type = TriangleMeshGeometryProvider<MeshType>;
  using const_iterator =
      typename TriangleMeshGeometryProvider<MeshType>::const_iterator;
  using storage_strategy =
      ElementProviderStorageStrategy<TriangleMeshGeometryProvider<MeshType>>;
};

template <>
struct VertexAttributeProviderTraits<Polyloop_2, PositionVertexAttribute> {
  using provider_type = PolyloopSimplicialAdaptor<Polyloop_2>;
  using const_iterator = PolyloopSimplicialAdaptor<Polyloop_2>::const_iterator;
  using storage_strategy =
      ElementProviderStorageStrategy<PolyloopSimplicialAdaptor<Polyloop_2>>;
};

template <>
struct VertexAttributeProviderTraits<GeometryPolyloop_3,
                                     PositionVertexAttribute> {
  using provider_type = PolyloopSimplicialAdaptor<GeometryPolyloop_3>;
  using const_iterator =
      PolyloopSimplicialAdaptor<GeometryPolyloop_3>::const_iterator;
  using storage_strategy = ElementProviderStorageStrategy<
      PolyloopSimplicialAdaptor<GeometryPolyloop_3>>;
};

#endif  // _FRAMEWORK_GEOMETRY_DEFAULT_VERTEX_ATTRIBUTE_PROVIDER_TRAITS_H_
