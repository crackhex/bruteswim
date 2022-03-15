#ifndef MATHS_H
#define MATHS_H

#include "types.h"

extern s16 atan2s(f32 y, f32 x);
void *vec3f_copy(f32 dest[3], f32 src[3]);
void *vec3f_set(f32 dest[3], f32 x, f32 y, f32 z);

#define sins(x) sin_table[(u16) (x) >> 4]
#define coss(x) sin_table[(u16) ((x) + 0x4000) >> 4]

#define min(a, b) ((a) <= (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))

#endif //MATHS_H

