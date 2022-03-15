#ifndef TYPES_H
#define TYPES_H

#include <stddef.h>
#include <stdint.h>


#define TRUE 1
#define FALSE 0
typedef signed char            s8;
typedef unsigned char          u8;
typedef signed short int       s16;
typedef unsigned short int     u16;
typedef signed int             s32;
typedef unsigned int           u32;
typedef int64_t s64;
typedef uint64_t u64;

typedef float  f32;
typedef double f64;

extern f32 sin_table[];
extern s16 arctan_table[];
//extern s16 atan2s(f32 y, f32 x);

struct Controller {
    /*0x00*/ s16 rawStickX;       //
    /*0x02*/ s16 rawStickY;       //
    /*0x04*/ float stickX;        // [-64, 64] positive is right
    /*0x08*/ float stickY;        // [-64, 64] positive is up
    /*0x0C*/ float stickMag;
};

struct Surface {
    /*0x00*/ s16 type;
    /*0x02*/ s16 force;
    /*0x04*/ s8 flags;
    /*0x05*/ s8 room;
    /*0x06*/ s16 lowerY;
    /*0x08*/ s16 upperY;
    /*0x0A*/ s16 vertex1[3];
    /*0x10*/ s16 vertex2[3];
    /*0x16*/ s16 vertex3[3];
    /*0x1C*/ struct {
        f32 x;
        f32 y;
        f32 z;
    } normal;
    /*0x28*/ f32 originOffset;
    /*0x2C*/ struct Object *object;
};
struct MarioState {
    s16 intendedYaw;
    s16 slideYaw;
    f32 forwardVel;
    f32 intendedMag;
    f32 slideVelX;
    f32 slideVelZ;
    struct Surface *floor;
    f32 floorHeight;
    s16 floorAngle;
    s16 faceAngle[3];
    f32 pos[3];
    f32 vel[3];
    struct Controller *controller;
};
struct Input {
    s16 x;
    s16 y;
};
#endif //TYPES_H
