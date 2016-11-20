#ifndef _FRAMEWORK_GEOMETRY_SIMPLEX_TYPES_H_
#define _FRAMEWORK_GEOMETRY_SIMPLEX_TYPES_H_

/** The set of simplex types that are provided by simplicial adaptors. We
 * differentiate the manner in which the simplicial adaptors provide flat
 * iteration over the provided simplices. Thus, if a simplicial adaptor
 * provides triangles, but, its flattened iterator provides a central vertex
 * attribute, and then, a collection of vertices attributes that may be
 * interpreted to form a triangle fan, the simplex type is a TriangleFan,
 * while, if it provides iteration over separate triangles, the simplex type is
 * a TriangleList.
 */

struct PointList {};

struct LineList {};

struct TriangleFan {};

struct TriangleList {};

#endif  //_FRAMEWORK_GEOMETRY_SIMPLEX_TYPES_H_
