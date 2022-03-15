#ifndef SURFACE_H
#define SURFACE_H

#include "types.h"
s16 ptInTriangle(f32 p[3], s16 p0[3], s16 p1[3], s16 p2[3]);
struct Surface *read_surface_data(s16 **vertexData[], s16 triNum);
struct Surface * check_mario_surface(f32 mPos[3], struct Surface **triList, s16 numTris);
f32 find_floor_height(struct Surface *surf, s32 x, s32 y, s32 z);
#endif //SURFACE_H
