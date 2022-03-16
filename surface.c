#include "types.h"
#include "maths.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

s16 ptInTriangle(f32 p[3], s16 p0[3], s16 p1[3], s16 p2[3]) {
    if (( p0[2] - (s16) p[2]) * (p1[0] -  p0[0]) - (p0[0] - (s16) p[0]) * (p1[2] - p0[2]) > 0) {
        if (( p1[2] - (s16) p[2]) * ( p2[0] -  p1[0]) - ( p1[0] - (s16) p[0]) * ( p2[2] -  p1[2]) > 0) {
            if ((p2[2] - (s16) p[2]) * ( p0[0] -  p2[0]) - ( p2[0] - (s16) p[0]) * ( p0[2] - p2[2]) > 0) {
                return TRUE;
            }
        }
    }
    return FALSE;
}

struct Surface * init_surface_data(s16 vertexData[][3][3], s16 triNum) {
    struct Surface *surface = malloc(sizeof(struct Surface));
    s32 x1, y1, z1;
    s32 x2, y2, z2;
    s32 x3, y3, z3;
    s32 maxY, minY;
    f32 nx, ny, nz;
    f32 mag;

    x1 = vertexData[triNum][0][0];
    y1 = vertexData[triNum][0][1];
    z1 = vertexData[triNum][0][2];

    x2 = vertexData[triNum][1][0];
    y2 = vertexData[triNum][1][1];
    z2 = vertexData[triNum][1][2];

    x3 = vertexData[triNum][2][0];
    y3 = vertexData[triNum][2][1];
    z3 = vertexData[triNum][2][2];
    // (v2 - v1) x (v3 - v2)
    nx = (f32) (y2 - y1) * (f32) (z3 - z2) - (f32) (z2 - z1) * (f32) (y3 - y2);
    ny = (f32) (z2 - z1) * (f32) (x3 - x2) - (f32) (x2 - x1) * (f32) (z3 - z2);
    nz = (f32) (x2 - x1) * (f32) (y3 - y2) - (f32) (y2 - y1) * (f32) (x3 - x2);

    mag = sqrtf(nx * nx + ny * ny + nz * nz);
    // Could have used min_3 and max_3 for this...
    minY = y1;
    if (y2 < minY) {
        minY = y2;
    }
    if (y3 < minY) {
        minY = y3;
    }

    maxY = y1;
    if (y2 > maxY) {
        maxY = y2;
    }
    if (y3 > maxY) {
        maxY = y3;
    }

    // Checking to make sure no DIV/0
    if (mag < 0.0001) {
        return NULL;
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

    return surface;

}

struct Surface * init_ceil_data(s16 vertexData[][3][3], s16 triNum) {
    struct Surface *surf = malloc(sizeof(struct Surface));
    struct Surface *ceil = NULL;
    ceil = NULL;
    s32 x1 = vertexData[triNum][0][0];
    s32 y1 = vertexData[triNum][0][1];
    s32 z1 = vertexData[triNum][0][2];
    s32 x2 = vertexData[triNum][1][0];
    s32 y2 = vertexData[triNum][1][1];
    s32 z2 = vertexData[triNum][1][2];
    s32 x3 = vertexData[triNum][2][0];
    s32 y3 = vertexData[triNum][2][1];
    s32 z3 = vertexData[triNum][2][2];

    f32 nx = (f32) (y2 - y1) * (f32) (z3 - z2) - (f32) (z2 - z1) * (f32) (y3 - y2);
    f32 ny = (f32) (z2 - z1) * (f32) (x3 - x2) - (f32) (x2 - x1) * (f32) (z3 - z2);
    f32 nz = (f32) (x2 - x1) * (f32) (y3 - y2) - (f32) (y2 - y1) * (f32) (x3 - x2);
    f32 oo = surf->originOffset;
    surf->vertex1[0] = x1;
    surf->vertex2[0] = x2;
    surf->vertex3[0] = x3;

    surf->vertex1[1] = y1;
    surf->vertex2[1] = y2;
    surf->vertex3[1] = y3;

    surf->vertex1[2] = z1;
    surf->vertex2[2] = z2;
    surf->vertex3[2] = z3;

    surf->normal.x = nx;
    surf->normal.y = ny;
    surf->normal.z = nz;

    surf->originOffset = -(nx * x1 + ny * y1 + nz * z1);

    /*surf->lowerY = minY - 5;
    surf->upperY = maxY + 5;*/
}

struct Surface * check_mario_surface(f32 mPos[3], struct Surface **triList, s16 numTris) {
    s16 i;
    //printf("%i\n", numTris);
    struct Surface *surface;
    for (i = 0; i < numTris; i++) {
        surface = triList[i];
        //numSurfaces = sizeof vertexData / sizeof *vertexData;
        if (ptInTriangle( mPos, surface->vertex1, surface->vertex2, surface->vertex3) == 1) {
            //printf("%i\n", i);
            return surface;
        }
        //surface->type = surfaceType;
    }
    return surface;

}
struct Surface * check_mario_ceil(f32 mPos[3], struct Surface **triList, s16 numTris, f32 *pheight) {
    s16 i;
    f32 nx, ny, nz;
    f32 oo;
    f32 height;
    //printf("%i\n", numTris);
    struct Surface *surface;
    for (i = 0; i < numTris; i++) {
        surface = triList[i];
        nx = surface->normal.x;
        ny = surface->normal.y;
        nz = surface->normal.z;
        oo = surface->originOffset;
        height = -((f32) mPos[0] * nx + nz * (f32) mPos[2] + oo) / ny;
        //numSurfaces = sizeof vertexData / sizeof *vertexData;
        if (ptInTriangle( mPos, surface->vertex1, surface->vertex2, surface->vertex3) == 0) {
            //printf("%i\n", i);
            continue;
        }
        if (mPos[1] - (height - -78.0f) > 0.0f) {
            continue;
        }
        return surface;
        //surface->type = surfaceType;
    }
    *pheight = height;
    return surface;

}
f32 find_floor_height(struct Surface *surf, s32 x, s32 y, s32 z) {
    f32 nx, ny, nz;
    f32 oo;
    f32 height;

    nx = surf->normal.x;
    ny = surf->normal.y;
    nz = surf->normal.z;
    oo = surf->originOffset;
    height = -((f32) x * nx + nz * (f32) z + oo) / ny;
    return height;
}


