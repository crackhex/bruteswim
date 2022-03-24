#include "types.h"
#include "maths.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>


void init_surface_data(Surface *surface, s16 vertexData[][3][3], s16 triNum) {

    s32 x1 = vertexData[triNum][0][0];
    s32 y1 = vertexData[triNum][0][1];
    s32 z1 = vertexData[triNum][0][2];

    s32 x2 = vertexData[triNum][1][0];
    s32 y2 = vertexData[triNum][1][1];
    s32 z2 = vertexData[triNum][1][2];

    s32 x3 = vertexData[triNum][2][0];
    s32 y3 = vertexData[triNum][2][1];
    s32 z3 = vertexData[triNum][2][2];
    // (v2 - v1) x (v3 - v2)
    f32 nx = (f32) (y2 - y1) * (f32) (z3 - z2) - (f32) (z2 - z1) * (f32) (y3 - y2);
    f32 ny = (f32) (z2 - z1) * (f32) (x3 - x2) - (f32) (x2 - x1) * (f32) (z3 - z2);
    f32 nz = (f32) (x2 - x1) * (f32) (y3 - y2) - (f32) (y2 - y1) * (f32) (x3 - x2);

    f32 mag = sqrtf(nx * nx + ny * ny + nz * nz);
    // Could have used min_3 and max_3 for this...
    s32 minY = y1;
    if (y2 < minY) {
        minY = y2;
    }
    if (y3 < minY) {
        minY = y3;
    }

    s32 maxY = y1;
    if (y2 > maxY) {
        maxY = y2;
    }
    if (y3 > maxY) {
        maxY = y3;
    }

    // Checking to make sure no DIV/0
    if (mag < 0.0001) {
        return;
    }

    mag = (f32)(1.0 / mag);
    nx *= mag;
    ny *= mag;
    nz *= mag;
    //surface = alloc_surface();
    surface->vertex1[0] = x1;
    surface->vertex2[0] = x2;
    surface->vertex3[0] = x3;

    surface->vertex1[1] = y1;
    surface->vertex2[1] = y2;
    surface->vertex3[1] = y3;

    surface->vertex1[2] = z1;
    surface->vertex2[2] = z2;
    surface->vertex3[2] = z3;

    surface->normal.x = nx;
    surface->normal.y = ny;
    surface->normal.z = nz;

    surface->originOffset = -(nx * x1 + ny * y1 + nz * z1);

    surface->lowerY = minY - 5;
    surface->upperY = maxY + 5;

    if (surface->normal.x < -0.707 || surface->normal.x > 0.707) {
        surface->flags |= SURFACE_FLAG_X_PROJECTION;
    }

}
s32 check_wall_collisions(Surface *triList, s16 numTris,
                          WallCollisionData *data) {
    f32 radius = data->radius;
    f32 x = data->x;
    f32 y = data->y + data->offsetY;
    f32 z = data->z;
    f32 w1, w2, w3;
    f32 y1, y2, y3;
    s32 numCols = 0;

    // Max collision radius = 200
    if (radius > 200.0f) {
        radius = 200.0f;
    }

    // Stay in this loop until out of walls.
    for (s16 i = 0; i < numTris; i++) {
        Surface *surf = triList + i;
        // Exclude a large number of walls immediately to optimize.
        if (y < surf->lowerY || y > surf->upperY) {
            continue;
        }

        f32 offset = surf->normal.x * x + surf->normal.y * y + surf->normal.z * z + surf->originOffset;

        if (offset < -radius || offset > radius) {
            continue;
        }

        f32 px = x;
        f32 pz = z;

        //! (Quantum Tunneling) Due to issues with the vertices walls choose and
        //  the fact they are floating point, certain floating point positions
        //  along the seam of two walls may collide with neither wall or both walls.
        if (surf->flags & SURFACE_FLAG_X_PROJECTION) {
            w1 = (f32) -surf->vertex1[2];            w2 = (f32) -surf->vertex2[2];            w3 = (f32) -surf->vertex3[2];
            y1 = surf->vertex1[1];            y2 = surf->vertex2[1];            y3 = surf->vertex3[1];

            if (surf->normal.x > 0.0f) {
                if ((y1 - y) * (w2 - w1) - (w1 - -pz) * (y2 - y1) > 0.0f) {
                    continue;
                }
                if ((y2 - y) * (w3 - w2) - (w2 - -pz) * (y3 - y2) > 0.0f) {
                    continue;
                }
                if ((y3 - y) * (w1 - w3) - (w3 - -pz) * (y1 - y3) > 0.0f) {
                    continue;
                }
            } else {
                if ((y1 - y) * (w2 - w1) - (w1 - -pz) * (y2 - y1) < 0.0f) {
                    continue;
                }
                if ((y2 - y) * (w3 - w2) - (w2 - -pz) * (y3 - y2) < 0.0f) {
                    continue;
                }
                if ((y3 - y) * (w1 - w3) - (w3 - -pz) * (y1 - y3) < 0.0f) {
                    continue;
                }
            }
        } else {
            w1 = surf->vertex1[0];            w2 = surf->vertex2[0];            w3 = surf->vertex3[0];
            y1 = surf->vertex1[1];            y2 = surf->vertex2[1];            y3 = surf->vertex3[1];

            if (surf->normal.z > 0.0f) {
                if ((y1 - y) * (w2 - w1) - (w1 - px) * (y2 - y1) > 0.0f) {
                    continue;
                }
                if ((y2 - y) * (w3 - w2) - (w2 - px) * (y3 - y2) > 0.0f) {
                    continue;
                }
                if ((y3 - y) * (w1 - w3) - (w3 - px) * (y1 - y3) > 0.0f) {
                    continue;
                }
            } else {
                if ((y1 - y) * (w2 - w1) - (w1 - px) * (y2 - y1) < 0.0f) {
                    continue;
                }
                if ((y2 - y) * (w3 - w2) - (w2 - px) * (y3 - y2) < 0.0f) {
                    continue;
                }
                if ((y3 - y) * (w1 - w3) - (w3 - px) * (y1 - y3) < 0.0f) {
                    continue;
                }
            }
        }

        //! (Wall Overlaps) Because this doesn't update the x and z local variables,
        //  multiple walls can push mario more than is required.
        data->x += surf->normal.x * (radius - offset);
        data->z += surf->normal.z * (radius - offset);

        //! (Unreferenced Walls) Since this only returns the first four walls,
        //  this can lead to wall interaction being missed. Typically unreferenced walls
        //  come from only using one wall, however.
        if (data->numWalls < 4) {
            data->walls[data->numWalls++] = surf;
        }

        numCols++;
    }

    return numCols;
}

f32 check_mario_floor(f32 x, f32 y, f32 z, Surface *triList, s16 numTris, Surface **pfloor) {
    s32 x1, z1, x2, z2, x3, z3;
    f32 height = CELL_HEIGHT_LIMIT;
    //printf("%i\n", numTris);

    for (s16 i = 0; i < numTris; i++) {
        Surface *floor = triList + i;

        //numSurfaces = sizeof vertexData / sizeof *vertexData;
        x1 = triList[i].vertex1[0];
        z1 = triList[i].vertex1[2];
        x2 = triList[i].vertex2[0];
        z2 = triList[i].vertex2[2];
        if ((z1 - z) * (x2 - x1) - (x1 - x) * (z2 - z1) < 0) {
            continue;
        }

        // To slightly save on computation time, set this later.
        x3 = triList[i].vertex3[0];
        z3 = triList[i].vertex3[2];

        if ((z2 - z) * (x3 - x2) - (x2 - x) * (z3 - z2) < 0) {
            continue;
        }
        if ((z3 - z) * (x1 - x3) - (x3 - x) * (z1 - z3) < 0) {
            continue;
        }

        height = -((f32) x * floor->normal.x + floor->normal.z * (f32) z + floor->originOffset) / floor->normal.y;
        *pfloor = floor;
        //surface->type = surfaceType;
    }

    return height;

}
f32 check_mario_ceil(f32 x, f32 y, f32 z, Surface *triList, s16 numTris, Surface  **pceil) {
    s32 x1, z1, x2, z2, x3, z3;
    f32 height = CELL_HEIGHT_LIMIT;
    //printf("%i\n", numTris);
    for (s16 i = 1; i <= numTris; i++) {
        Surface *surf = triList + i;
        //numSurfaces = sizeof vertexData / sizeof *vertexData;
        x1 = triList[i].vertex1[0];
        z1 = triList[i].vertex1[2];
        x2 = triList[i].vertex2[0];
        z2 = triList[i].vertex2[2];
        if ((z1 - z) * (x2 - x1) - (x1 - x) * (z2 - z1) < 0) {
            continue;
        }

        // To slightly save on computation time, set this later.
        x3 = triList[i].vertex3[0];
        z3 = triList[i].vertex3[2];

        if ((z2 - z) * (x3 - x2) - (x2 - x) * (z3 - z2) < 0) {
            continue;
        }
        if ((z3 - z) * (x1 - x3) - (x3 - x) * (z1 - z3) < 0) {
            continue;
        }
        height = -((f32) x * surf->normal.x + surf->normal.z * (f32) z + surf->originOffset) / surf->normal.y;
        if (y - (height - -78.0f) > 0.0f) {
            continue;
        }
        *pceil = surf;
        return height;
        //surface->type = surfaceType;
    }
    return height;

}
f32 find_tri_height(Surface *surf, s32 x, s32 y, s32 z) {

    f32 height = -((f32) x * surf->normal.x + surf->normal.z * (f32) z + surf->originOffset) / surf->normal.y;
    return height;
}

Surface *resolve_and_return_wall_collisions(Surface *triList, s16 numTris, f32 pos[3], f32 offset, f32 radius) {
    WallCollisionData collisionData;
    Surface *wall = NULL;

    collisionData.x = pos[0];
    collisionData.y = pos[1];
    collisionData.z = pos[2];
    collisionData.radius = radius;
    collisionData.offsetY = offset;

    if (check_wall_collisions(triList, numTris, &collisionData)) {
        wall = collisionData.walls[collisionData.numWalls - 1];
    }

    pos[0] = collisionData.x;
    pos[1] = collisionData.y;
    pos[2] = collisionData.z;

    // This only returns the most recent wall and can also return NULL
    // there are no wall collisions.
    return wall;
}
f32 vec3f_find_ceil(f32 pos[3], f32 height, Surface **ceil) {

    return find_tri_height(*ceil, pos[0], height + 80.0f, pos[2]);
}

s16 find_floor_slope(MarioState *m, s16 yawOffset, Surface *triList) {
    Surface *floor;
    f32 forwardFloorY, backwardFloorY;
    f32 forwardYDelta, backwardYDelta;
    s16 result;

    f32 x = sins(m->faceAngle[1] + yawOffset) * 5.0f;
    f32 z = coss(m->faceAngle[1] + yawOffset) * 5.0f;

    forwardFloorY = check_mario_floor(m->pos[0] + x, m->pos[1] + 100.0f, m->pos[2] + z, triList, 2,&floor);
    backwardFloorY = check_mario_floor(m->pos[0] - x, m->pos[1] + 100.0f, m->pos[2] - z, triList, 2, &floor);

    //! If Mario is near OOB, these floorY's can sometimes be -11000.
    //  This will cause these to be off and give improper slopes.
    forwardYDelta = forwardFloorY - m->pos[1];
    backwardYDelta = m->pos[1] - backwardFloorY;

    if (forwardYDelta * forwardYDelta < backwardYDelta * backwardYDelta) {
        result = atan2s(5.0f, forwardYDelta);
    } else {
        result = atan2s(5.0f, backwardYDelta);
    }

    return result;
}

