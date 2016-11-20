#ifndef _FRAMEWORK_GEOMETRY_DEFAULT_VERTEX_ATTRIBUTE_PROVIDER_TRAITS_H_
#define _FRAMEWORK_GEOMETRY_DEFAULT_VERTEX_ATTRIBUTE_PROVIDER_TRAITS_H_

#include <CGAL/Polyhedron_3.h>

#include "polyline_3.h"
#include "polyloop_2.h"
#include "polyloop_3.h"
#include "simplicialAdaptors/polyloopSimplicialAdaptor.h"
#include "simplicialAdaptors/polylineSimplicialAdaptor.h"
#include "simplicialAdaptors/triangleMeshSimplicialAdaptor.h"

#include "vertexAttributeProviderTraits.h"

/**
 * There is a desire for the client to easily instantiate attribute providers
 * that work with the framework. It is specially important to be able to
 * specify quickly, vertex attribute providers - such providers are
 * representable as any ordered container - and, it should be possible for the
 * client to specify them as such. Now, the vertex attribute providers for
 * some geometry representations are difference from the representation itself.
 * The separation into simplicialAdaptor and geometry representation is what
 * allowed us to separate iteration over a particular geometry, and the
 * geometry's representation itself. We specialize the VertexAttributeProvider
 * traits for some common container types, so that these can be used as
 * VertexAttributeProviders implicitly.
 */

// In case the user passes a simple vector, the best guess is to interpret it
// as a list of ordered elements, and use a PolylineSimplicialAdaptor.
template <typename Attribute, typename SimplexType>
struct VertexAttributeProviderTraits<std::vector<Attribute>, SimplexType>
    : public ConstAttributeProviderTraits<
          PolylineSimplicialAdaptor<std::vector<Attribute>, SimplexType>> {
  using provided_type = std::vector<Attribute>;
};

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

template <typename Kernel, typename Attribute, typename SimplexType>
struct VertexAttributeProviderTraits<CGAL::Polyhedron_3<Kernel>>
    : public ConstAttributeProviderTraits<TriangleMeshSimplicialAdaptor<
          CGAL::Polyhedron_3<Kernel>, SimplexType>> {
 private:
  using MeshType = CGAL::Polyhedron_3<Kernel>;

 public:
  using provided_type = MeshType;
};

template <typename SimplexType>
struct VertexAttributeProviderTraits<Polyloop_2, SimplexType> : public 
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
