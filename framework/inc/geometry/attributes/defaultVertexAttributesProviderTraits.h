#ifndef _FRAMEWORK_GEOMETRY_DEFAULT_VERTEX_ATTRIBUTES_PROVIDER_TRAITS_H_
#define _FRAMEWORK_GEOMETRY_DEFAULT_VERTEX_ATTRIBUTES_PROVIDER_TRAITS_H_

#include <CGAL/Polyhedron_3.h>

#include "polyline_3.h"
#include "polyloop_2.h"
#include "polyloop_3.h"
#include "simplicialAdaptors/polyloopSimplicialAdaptor.h"
#include "simplicialAdaptors/polylineSimplicialAdaptor.h"
#include "simplicialAdaptors/triangleMeshSimplicialAdaptor.h"

#include "vertexAttributesProviderTraits.h"

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

// In case the user passes in a tuple of a vector, and an attribute type,
// use a PolylineSimplicialAdaptor.
template <typename Attribute>
    struct VertexElementProviderTraits <
    std::tuple<Attribute, std::vector<attributes_to_types_t<Attribute>>>
    : public ConstAttributesProviderTraits <
      PolylineSimplicialAdaptor<std::vector<attributes_to_types_t<Attribute>>> {
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

template <>
struct VertexAttributesProviderTraits<Polyloop_2>
    : public ConstAttributeProviderTraits<
          PolylineSimplicialAdaptor<Polyloop_2>> {};

template <typename VertexBase, typename EdgeBase>
    struct VertexElementProviderTraits <
    Polyloop_3<VertexBase, PositionVertexAttribute> {
  using provider_type = PolyloopSimplicialAdaptor<GeometryPolyloop_3>;
  using const_iterator =
      PolyloopSimplicialAdaptor<GeometryPolyloop_3>::const_iterator;
  using storage_strategy = ElementProviderStorageStrategy<
      PolyloopSimplicialAdaptor<GeometryPolyloop_3>>;
};

#endif  // _FRAMEWORK_GEOMETRY_DEFAULT_VERTEX_ATTRIBUTES_PROVIDER_TRAITS_H_
