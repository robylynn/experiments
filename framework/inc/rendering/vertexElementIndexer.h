#ifndef _FRAMEWORK_RENDERING_VERTEX_ELEMENT_INDEXER_H_
#define _FRAMEWORK_RENDERING_VERTEX_ELEMENT_INDEXER_H_

#include <glog/logging.h>

#include <CGAL/Point_2.h>

// Indexes a particular coordinate of a vertex element (which is of type
// CGAL::Point_3, CGAL::Point_2, CGAL::Color, etc.
//
// We specialize for the above types, where a simple operator[] doesn't do the
// job.
template <typename IterRefType>
class VertexElementIndexer {
 public:
  auto operator()(const IterRefType& ref, size_t coordinateIndex)
      -> decltype(std::declval<IterRefType>()[0]) const {
    return ref[coordinateIndex];
  }
};

template <typename K>
class VertexElementIndexer<const CGAL::Point_2<K>> {
 public:
  auto operator()(const CGAL::Point_2<K>& point, size_t coordinateIndex)
      -> decltype(std::declval<CGAL::Point_2<K>>()[0]) const {
    LOG_IF(ERROR, coordinateIndex > 3) << "More than 3 indexes indexed for "
                                          "Kernel::Point_2. This may indicate "
                                          "a bug";
    return (coordinateIndex < 2)
               ? point[coordinateIndex]
               : static_cast<decltype(std::declval<CGAL::Point_2<K>>()[0])>(0);
  }
};

#endif  //_FRAMEWORK_RENDERING_VERTEX_ELEMENT_INDEXER_H_
