//
// Created by axel on 14/11/17.
//

#include "Algorithm.h"

namespace Algorithms{
    coord_t MonotoneChain::cross(const Point O, const Point A, const Point B)
    {
        return (A.x - O.x) * (B.y - O.y) - (A.y - O.y) * (B.x - O.x);
    }

    vector<Point> MonotoneChain::convexHull(vector<Point> pList){
        long n = pList.size(), k = 0;
        if (n == 1) return pList;
        vector<Point> hullPointList(2*n);

        // Sort points lexicographically
        sort(pList.begin(), pList.end());

        // Build lower hull
        for (int i = 0; i < n; ++i) {
            while (k >= 2 && cross(hullPointList[k-2], hullPointList[k-1], pList[i]) <= 0){
                k--;
            }
            hullPointList[k++] = pList[i];
        }

        // Build upper hull
        for (long i = n-2, t = k+1; i >= 0; i--) {
            while (k >= t && MonotoneChain::cross(hullPointList[k-2], hullPointList[k-1], pList[i]) <= 0){
                k--;
            }
            hullPointList[k++] = pList[i];
        }

        hullPointList.resize(k-1);
        return hullPointList;
    }
}