#ifndef SURFACE_H
#define SURFACE_H

#include "types.h"

s16 ptInTriangle(f32 p[3], s16 p0[3], s16 p1[3], s16 p2[3]);
struct Surface *init_surface_data(s16 **vertexData[], s16 triNum);
struct Surface *init_ceil_data(s16 **vertexData[], s16 triNum);
f32 check_mario_floor(f32 mPos[3], struct Surface **triList, s16 numTris, struct Surface **pfloor);
f32 check_mario_ceil(f32 mPos[3], struct Surface **triList, s16 numTris, struct Surface **pceil);
f32 vec3f_find_ceil(pos[3], f32 height, struct Surface **ceil);
#endif //SURFACE_H
