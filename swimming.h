#ifndef SWIMMING_H
#define SWIMMING_H

#include "types.h"

void common_swimming_step(MarioState *m, s16 swimStrength, Surface *floorList, Surface *wallList, Surface *ceilList, s16 numFlos, s16 numWalls, s16 numCeils);
s32 mario_execute_submerged_action(struct MarioState *m, Surface *floorList, Surface *wallList, Surface *ceilList, s16 numFlos, s16 numWalls, s16 numCeils);

#endif //SWIMMING_H
