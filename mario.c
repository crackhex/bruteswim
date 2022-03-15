#include "types.h"
#include "maths.h"
#include <math.h>

void update_mario_joystick_inputs(struct MarioState *m) {
    struct Controller *controller = m->controller;
    controller->stickMag = sqrtf(controller->stickX * controller->stickX + controller->stickY * controller->stickY);
    f32 mag = ((controller->stickMag / 64.0f) * (controller->stickMag / 64.0f)) * 64.0f;


    m->intendedMag = mag / 2.0f;

    if (m->intendedMag > 0.0f) {
        m->intendedYaw = atan2s(-controller->stickY, controller->stickX);
    } else {
        m->intendedYaw = m->faceAngle[1];
    }
}