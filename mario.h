#ifndef MARIO_H
#define MARIO_H

#include "types.h"

void update_mario_joystick_inputs(MarioState *m);
u32 set_mario_action(MarioState *m, u32 action, u32 actionArg);
s32 execute_mario_action(MarioState *m, Surface *floorList, Surface *wallList, Surface *ceilList, s16 numFlos, s16 numWalls, s16 numCeils);

#endif //MARIO_H
