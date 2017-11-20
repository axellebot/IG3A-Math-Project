//
// Created by Axel LE BOT, Andrew LENC on 14/11/17.
//

#ifndef SOURCES_POINT_H
#define SOURCES_POINT_H

#include <stdlib.h>
using namespace std;

typedef double coord_t;
typedef struct point {
    coord_t x, y;
    bool operator<(const struct point p)const {
        return x < p.x || (x == p.x && y < p.y);
    }
} Point;

#endif //SOURCES_POINT_H
