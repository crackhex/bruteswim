#include "types.h"
#include "maths.h"
#include "swimming.h"
#include <math.h>

void update_mario_joystick_inputs(MarioState *m) {
    Controller *controller = m->controller;
    controller->stickMag = sqrtf(controller->stickX * controller->stickX + controller->stickY * controller->stickY);
    f32 mag = ((controller->stickMag / 64.0f) * (controller->stickMag / 64.0f)) * 64.0f;


    m->intendedMag = mag / 2.0f;

    if (m->intendedMag > 0.0f) {
        m->intendedYaw = atan2s(-controller->stickY, controller->stickX);
    } else {
        m->intendedYaw = m->faceAngle[1];
    }
}

u32 set_mario_action(struct MarioState *m, u32 action, u32 actionArg) {

    // Initialize the action information.
    m->prevAction = m->action;
    m->action = action;
    m->actionArg = actionArg;
    m->actionState = 0;
    m->actionTimer = 0;

    return TRUE;
}
s32 execute_mario_action(MarioState *m, Surface *floorList, Surface *wallList, Surface *ceilList, s16 numFlos, s16 numWalls, s16 numCeils) {
    s32 inLoop = TRUE;

    if (m->action) {
        //update_mario_inputs(m);
        // If Mario is OOB, stop executing actions.
        if (m->floor == NULL) {
            return 0;
        }

        // The function can loop through many action shifts in one frame,
        // which can lead to unexpected sub-frame behavior. Could potentially hang
        // if a loop of actions were found, but there has not been a situation found.
        while (inLoop) {
            switch (m->action & ACT_GROUP_MASK) {
                case ACT_GROUP_SUBMERGED:
                    inLoop = mario_execute_submerged_action(m, floorList, wallList, ceilList, numFlos, numWalls, numCeils);
                    break;
            }
        }
    }

    return 0;
}

