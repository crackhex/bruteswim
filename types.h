#ifndef TYPES_H
#define TYPES_H

#include <stddef.h>
#include <stdint.h>


#define TRUE 1
#define FALSE 0

#define CELL_HEIGHT_LIMIT           20000
#define FLOOR_LOWER_LIMIT           -11000

#define INPUT_NONZERO_ANALOG         0x0001
#define INPUT_A_PRESSED              0x0002
#define INPUT_A_DOWN                 0x0080
#define INPUT_IN_WATER               0x0200

#define SURFACE_FLAG_X_PROJECTION     (1 << 3)

#define WATER_STEP_NONE        0
#define WATER_STEP_HIT_FLOOR   1
#define WATER_STEP_HIT_CEILING 2
#define WATER_STEP_CANCELLED   3
#define WATER_STEP_HIT_WALL    4

#define ACT_GROUP_MASK       0x000001C0
#define ACT_GROUP_SUBMERGED  /* 0x000000C0 */ (3 << 6)
#define ACT_FLAG_STATIONARY                  /* 0x00000200 */ (1 <<  9)
#define ACT_FLAG_MOVING                      /* 0x00000400 */ (1 << 10)

#define ACT_FLAG_SWIMMING                    /* 0x00002000 */ (1 << 13)
#define ACT_FLAG_PAUSE_EXIT                  /* 0x08000000 */ (1 << 27)
#define ACT_FLAG_SWIMMING_OR_FLYING          /* 0x10000000 */ (1 << 28)
#define ACT_FLAG_WATER_OR_TEXT               /* 0x20000000 */ (1 << 29)

// group 0x0C0: submerged actions
#define ACT_WATER_IDLE                 0x380022C0 // (0x0C0 | ACT_FLAG_STATIONARY | ACT_FLAG_SWIMMING | ACT_FLAG_PAUSE_EXIT | ACT_FLAG_SWIMMING_OR_FLYING | ACT_FLAG_WATER_OR_TEXT)
#define ACT_WATER_ACTION_END           0x300022C2 // (0x0C2 | ACT_FLAG_STATIONARY | ACT_FLAG_SWIMMING | ACT_FLAG_SWIMMING_OR_FLYING | ACT_FLAG_WATER_OR_TEXT)
#define ACT_BREASTSTROKE               0x300024D0 // (0x0D0 | ACT_FLAG_MOVING | ACT_FLAG_SWIMMING | ACT_FLAG_SWIMMING_OR_FLYING | ACT_FLAG_WATER_OR_TEXT)
#define ACT_SWIMMING_END               0x300024D1 // (0x0D1 | ACT_FLAG_MOVING | ACT_FLAG_SWIMMING | ACT_FLAG_SWIMMING_OR_FLYING | ACT_FLAG_WATER_OR_TEXT)
#define ACT_FLUTTER_KICK               0x300024D2 // (0x0D2 | ACT_FLAG_MOVING | ACT_FLAG_SWIMMING | ACT_FLAG_SWIMMING_OR_FLYING | ACT_FLAG_WATER_OR_TEXT)

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

typedef struct Controller {
    /*0x00*/ s16 rawStickX;       //
    /*0x02*/ s16 rawStickY;       //
    /*0x04*/ float stickX;        // [-64, 64] positive is right
    /*0x08*/ float stickY;        // [-64, 64] positive is up
    /*0x0C*/ float stickMag;
} Controller;

typedef struct WallCollisionData {
    /*0x00*/ f32 x, y, z;
    /*0x0C*/ f32 offsetY;
    /*0x10*/ f32 radius;
    /*0x14*/ u8 filler[2];
    /*0x16*/ s16 numWalls;
    /*0x18*/ struct Surface *walls[4];
} WallCollisionData;

typedef struct Surface {
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
} Surface;

typedef struct MarioState {
    u16 input;
    u32 action;
    u32 prevAction;
    u8 framesSinceA;
    u16 actionState;
    u16 actionTimer;
    u32 actionArg;
    s16 intendedYaw;
    f32 forwardVel;
    f32 intendedMag;
    struct Surface *wall;
    struct Surface *ceil;
    struct Surface *floor;
    f32 floorHeight;
    s16 floorAngle;
    s16 waterLevel;
    s16 faceAngle[3];
    s16 angleVel[3];
    f32 pos[3];
    f32 vel[3];
    struct Controller *controller;
} MarioState;

typedef struct Input {
    s16 x;
    s16 y;
} Input;
#endif //TYPES_H
