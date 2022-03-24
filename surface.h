#ifndef SURFACE_H
#define SURFACE_H

#include "types.h"

void init_surface_data(Surface *surface, s16 **vertexData[], s16 triNum);
f32 check_mario_floor(f32 x, f32 y, f32 z, Surface *triList, s16 numTris, Surface **pfloor);
f32 check_mario_ceil(f32 x, f32 y, f32 z, Surface *triList, s16 numTris, Surface **pceil);
s32 check_wall_collisions(Surface *triList, s16 numTris, WallCollisionData *data);
Surface *resolve_and_return_wall_collisions(Surface *triList, s16 numTris, f32 pos[3], f32 offset, f32 radius);
f32 vec3f_find_ceil(f32 pos[3], f32 height, Surface *ceil);
s16 find_floor_slope(MarioState *m, s16 yawOffset, Surface *triList);
#endif //SURFACE_H
