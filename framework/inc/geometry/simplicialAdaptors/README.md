Simplicial adaptors understand geometry representations enough to be able to
generate simplicial decompositions of them. A simplicial decomposition allows
for ordered iteration over vertices, and also tags the type of decomposition
it is providing (PointList, LineList, TriangleList, etc).

A particular simplicial provider will only provide a set of simplicial
decompositions (those that are natural to the geometry representation that the
provider is associated with). The simplicial adaptors only contain
combinatorial information, and thus, are only useful as adaptors over a
geometry representation (specially for handoff to rasterization based rendering
systems).

All simplicial adaptors provide:
provided_simplex - The type of the simplex that is provided.
const_iterator - The type of the const_iterator provided by begin and end.
begin, end - const iterators over a flat container of values.
size - The number of entries provided by geometry representation being adapted
by the simplicial provider.

The simplicial adaptors serve two chief purposes:
a) Separate specification of a geometry representation's attribute information
from iteration over the information according to a simplicial decomposition.
Thus, similar to the iterator design pattern, the combinatorial iteration state
is separated from the geometry representation itself. The geometry
representation need only provide access to the underlying information stored in
them in a geometry representation specific format. The conversion to a coherent
interface for flat iteration is provided by the representations simplicial
adaptor.
b) Facilitate templated hookup with the render pipeline by associating geometry
representations with simplex types. In some cases, flat data in the unrolled
format of a geometry representation may be created already, and the client may
simply desire to tag this for consumption according to the geometry
representation, without having to create the geometry representation object
itself (for example, the client may create a std::vector of Points that she
desires to be interpreted by the render engine as a list of line segments, two
points taken at a time). The client can simply create a renderable out of this
flat representation after tagging it as a LineList, and, an appropriate
simplifical adaptor is automatically instantiated based on the tag.
