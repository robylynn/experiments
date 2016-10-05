#include "cuboidGeometryProvider.h"

constexpr int AlignedCuboidTrianglesBuilder::TRIANGLES_PER_CUBOID;

const int AlignedCuboidTrianglesBuilder::s_triangleLUT
    [AlignedCuboidTrianglesBuilder::TRIANGLES_PER_CUBOID]
    [VERTICES_PER_TRIANGLE] = {{0, 1, 2},  // Back
                               {0, 2, 3},
                               {0, 3, 4},  // Left
                               {0, 4, 5},
                               {2, 3, 7},  // Top
                               {3, 4, 7},
                               {4, 5, 6},  // Front
                               {4, 6, 7},
                               {1, 2, 7},  // Right
                               {1, 7, 6},
                               {0, 1, 6},  // Bottom
                               {0, 6, 5}};
