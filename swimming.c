#include "surface.h"
#include "types.h"
#include "maths.h"


s32 swimming_near_surface(MarioState *m) {

    return ((f32) m->waterLevel - 80) - m->pos[1] < 400.0f;
}

f32 get_buoyancy(MarioState *m) {
    f32 buoyancy = 0.0f;

    if (swimming_near_surface(m)) {
        buoyancy = 1.25f;
    } else if (!(m->action & ACT_FLAG_MOVING)) {
        buoyancy = -2.0f;
    }

    return buoyancy;
}

void update_swimming_speed(MarioState *m, f32 decelThreshold) {
    f32 buoyancy = get_buoyancy(m);
    f32 maxSpeed = 28.0f;

    if (m->action & ACT_FLAG_STATIONARY) {
        m->forwardVel -= 2.0f;
    }

    if (m->forwardVel < 0.0f) {
        m->forwardVel = 0.0f;
    }

    if (m->forwardVel > maxSpeed) {
        m->forwardVel = maxSpeed;
    }

    if (m->forwardVel > decelThreshold) {
        m->forwardVel -= 0.5f;
    }

    m->vel[0] = m->forwardVel * coss(m->faceAngle[0]) * sins(m->faceAngle[1]);
    m->vel[1] = m->forwardVel * sins(m->faceAngle[0]) + buoyancy;
    m->vel[2] = m->forwardVel * coss(m->faceAngle[0]) * coss(m->faceAngle[1]);
}

void update_swimming_yaw(MarioState *m) {
    s16 targetYawVel = (s16) -(10.0f * m->controller->stickX);

    if (targetYawVel > 0) {
        if (m->angleVel[1] < 0) {
            m->angleVel[1] += 0x40;
            if (m->angleVel[1] > 0x10) {
                m->angleVel[1] = 0x10;
            }
        } else {
            m->angleVel[1] = (s16) approach_s32(m->angleVel[1], targetYawVel, 0x10, 0x20);
        }
    } else if (targetYawVel < 0) {
        if (m->angleVel[1] > 0) {
            m->angleVel[1] -= 0x40;
            if (m->angleVel[1] < -0x10) {
                m->angleVel[1] = -0x10;
            }
        } else {
            m->angleVel[1] = (s16) approach_s32(m->angleVel[1], targetYawVel, 0x20, 0x10);
        }
    } else {
        m->angleVel[1] = (s16) approach_s32(m->angleVel[1], 0, 0x40, 0x40);
    }

    m->faceAngle[1] += m->angleVel[1];
    m->faceAngle[2] = -m->angleVel[1] * 8;
}

static void update_swimming_pitch(MarioState *m) {
    s16 targetPitch = (s16) -(252.0f * m->controller->stickY);

    s16 pitchVel;
    if (m->faceAngle[0] < 0) {
        pitchVel = 0x100;
    } else {
        pitchVel = 0x200;
    }

    if (m->faceAngle[0] < targetPitch) {
        if ((m->faceAngle[0] += pitchVel) > targetPitch) {
            m->faceAngle[0] = targetPitch;
        }
    } else if (m->faceAngle[0] > targetPitch) {
        if ((m->faceAngle[0] -= pitchVel) < targetPitch) {
            m->faceAngle[0] = targetPitch;
        }
    }
}

static u32 perform_water_step(MarioState *m, Surface *floorList, Surface *wallList, Surface *ceilList, s16 numFlos, s16 numWalls, s16 numCeils) {
    //UNUSED u8 filler[4];
    u32 stepResult;
    f32 nextPos[3];
    f32 step[3];
    //struct Object *marioObj = m->marioObj;

    vec3f_copy(step, m->vel);

    /*if (m->action & ACT_FLAG_SWIMMING) {
        apply_water_current(m, step);
    } MAYBE IMPLEMENT SOON */

    nextPos[0] = m->pos[0] + step[0];
    nextPos[1] = m->pos[1] + step[1];
    nextPos[2] = m->pos[2] + step[2];

    if (nextPos[1] > m->waterLevel - 80) {
        nextPos[1] = m->waterLevel - 80;
        m->vel[1] = 0.0f;
    }

    //stepResult = perform_water_full_step(m, nextPos);
    struct Surface *wall;
    struct Surface *ceil;
    struct Surface *floor;
    f32 ceilHeight;
    f32 floorHeight;

    wall = resolve_and_return_wall_collisions(wallList, numWalls,nextPos, 10.0f, 110.0f);
    floorHeight = check_mario_floor(nextPos[0], nextPos[1], nextPos[2], floorList,numFlos, &floor);
    ceilHeight = vec3f_find_ceil(nextPos, floorHeight, ceil);

    if (floor == NULL) {
        return WATER_STEP_CANCELLED;
    }

    if (nextPos[1] >= floorHeight) {
        if (ceilHeight - nextPos[1] >= 160.0f) {
            vec3f_copy(m->pos, nextPos);
            m->floor = floor;
            m->floorHeight = floorHeight;

            if (wall != NULL) {
                return WATER_STEP_HIT_WALL;
            } else {
                return WATER_STEP_NONE;
            }
        }

        if (ceilHeight - floorHeight < 160.0f) {
            return WATER_STEP_CANCELLED;
        }

        //! Water ceiling downwarp
        vec3f_set(m->pos, nextPos[0], ceilHeight - 160.0f, nextPos[2]);
        m->floor = floor;
        m->floorHeight = floorHeight;
        return WATER_STEP_HIT_CEILING;
    } else {
        if (ceilHeight - floorHeight < 160.0f) {
            return WATER_STEP_CANCELLED;
        }

        vec3f_set(m->pos, nextPos[0], floorHeight, nextPos[2]);
        m->floor = floor;
        m->floorHeight = floorHeight;
        return WATER_STEP_HIT_FLOOR;
    }
    /*vec3f_copy(marioObj->header.gfx.pos, m->pos);
    vec3s_set(marioObj->header.gfx.angle, -m->faceAngle[0], m->faceAngle[1], m->faceAngle[2]);
    UNNECESSARY CODE */

    return stepResult;
}
void common_swimming_step(MarioState *m, s16 swimStrength, Surface *floorList, Surface *wallList, Surface *ceilList, s16 numFlos, s16 numWalls, s16 numCeils) {
    s16 floorPitch;

    update_swimming_yaw(m);
    update_swimming_pitch(m);
    update_swimming_speed(m, swimStrength / 10.0f);

    switch (perform_water_step(m, floorList, wallList, ceilList, numFlos, numWalls, numCeils)) {
        case WATER_STEP_HIT_FLOOR:
            floorPitch = (s16) -find_floor_slope(m, -0x8000, floorList);
            if (m->faceAngle[0] < floorPitch) {
                m->faceAngle[0] = floorPitch;
            }
            break;

        case WATER_STEP_HIT_CEILING:
            if (m->faceAngle[0] > -0x3000) {
                m->faceAngle[0] -= 0x100;
            }
            break;

        case WATER_STEP_HIT_WALL:
            if (m->controller->stickY == 0.0f) {
                if (m->faceAngle[0] > 0.0f) {
                    m->faceAngle[0] += 0x200;
                    if (m->faceAngle[0] > 0x3F00) {
                        m->faceAngle[0] = 0x3F00;
                    }
                } else {
                    m->faceAngle[0] -= 0x200;
                    if (m->faceAngle[0] < -0x3F00) {
                        m->faceAngle[0] = -0x3F00;
                    }
                }
            }
            break;
    }

    //surface_swim_bob(m);
}