#ifndef _FRAMEWORK_GEOMETRY_DEFAULT_GEOMETRY_PROVIDERS_H_
#define _FRAMEWORK_GEOMETRY_DEFAULT_GEOMETRY_PROVIDERS_H_

#include <CGAL/Polyhedron_3.h>

#include "polyline.h"
#include "polyloop_2.h"
#include "polyloop_3.h"
#include "polyloopGeometryProvider.h"
#include "polylineGeometryProvider.h"
#include "triangleMeshGeometryProvider.h"
#include "vertexElementProviderTraits.h"
#include "defaultElementTypes.h"

// A geometry interpretation tag allows geometry to be understood in a
// particular type, and an appropriate geometry provider used, that understands
// that type. This is particularly useful for sending across a flat list of
// geometry in a container, and automatically picking a geometry interpretor that
// will understand it as being ordered in accordance with a particular
// GeometryRepresentation. This allows for interpreting for example, a vector
// of points as vectors oriented from the origin to the point; understanding a
// list of points to denote a disc geometry, consisting of first the center,
// and then subsequent vertices in order around the disc, etc.
template <typename GeometryInterpretationTag, typename T>
struct VertexElementProviderTraits<
    std::tuple<GeometryInterpretationTag, std::vector<T>>>,
    typename VertexElementFromType<T>::type > {
  using provider_type =
      GeometryProviderFromTag<GeometryInterpretationTag>::type;
  using const_iterator =
      typename PolylineGeometryProvider<Polyline<PointType>>::const_iterator;
  using storage_strategy = ElementProviderStorageStrategy<
      PolylineGeometryProvider<Polyline<PointType>>>;
};

// In case the user passes a simple vector, the best guess is to interpret it
// as a list of ordered elements, and use a PolylineGeometryProvider.
template <typename T>
struct VertexElementProviderTraits<std::vector<T>,
                                   typename VertexElementFromType<T>::type> {
  using provider_type = PolylineGeometryProvider<std::vector<T>>;
  using const_iterator = typename provider_type::const_iterator;
  using storage_strategy = ElementProviderStorageStrategy<provider_type>;
};

template <typename Kernel, typename VertexElement>
struct VertexElementProviderTraits<CGAL::Polyhedron_3<Kernel>, VertexElement> {
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
struct VertexElementProviderTraits<Polyloop_2, PositionVertexElement> {
  using provider_type = PolyloopGeometryProvider<Polyloop_2>;
  using const_iterator = PolyloopGeometryProvider<Polyloop_2>::const_iterator;
  using storage_strategy =
      ElementProviderStorageStrategy<PolyloopGeometryProvider<Polyloop_2>>;
};

template <>
struct VertexElementProviderTraits<Polyloop_3, PositionVertexElement> {
  using provider_type = PolyloopGeometryProvider<Polyloop_3>;
  using const_iterator = PolyloopGeometryProvider<Polyloop_3>::const_iterator;
  using storage_strategy =
      ElementProviderStorageStrategy<PolyloopGeometryProvider<Polyloop_2>>;
};

#endif  // _FRAMEWORK_GEOMETRY_DEFAULT_GEOMETRY_PROVIDERS_H_
