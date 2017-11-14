//
// Created by axel on 14/11/17.
//

#ifndef SOURCES_ALGORITHM_H
#define SOURCES_ALGORITHM_H

#include <algorithm>
#include <vector>
#include "Point.h"

namespace Algorithms {
    // Implementation of Andrew's monotone chain 2D convex hull algorithm.
    class MonotoneChain {
    public:
        // 2D cross product of OA and OB vectors, i.e. z-component of their 3D cross product.
        // Returns a positive value, if OAB makes a counter-clockwise turn,
        // negative for clockwise turn, and zero if the points are collinear.
        static coord_t cross(const Point &O, const Point &A, const Point &B);

        // Returns a list of points on the convex hull in counter-clockwise order.
        // Note: the last point in the returned list is the same as the first one.
        static vector<Point> convexHull(vector<Point> pList);
    };
}


#endif //SOURCES_ALGORITHM_H
