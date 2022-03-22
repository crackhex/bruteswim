#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "mario.h"
#include "surface.h"
//! Define triangle points   (CHANGE)

//s16 tri1[3][3] = { {6144, 1024, 6144}, {7168, 2458, 8192}, {6144, 1024, -1535} };
s16 flo1[3][3] = {{-5375,0,6464},{-5842,128,6248},{-6015,0,6464}};
s16 flo2[3][3] = { {4144, 1024, 6144}, {4168, 2458, 8192}, {6144, 1024, -1535} };
s16 numFlos = 2;
s16 numCeils = 2;
s16 *floList[3][3];
s16 *ceilList[3][3];





//! Number of frames for swimming   (CHANGE)
int frames = 10;

//! Define Mario's position and hspd    (CHANGE)
f32 mPos[3] = {-5841.02685546875f, 55.1111145019531f, 6371.7158203125f};//{6309, 1255, 1336};
f32 vel = 0.0f;//50.131f;
s16 faceAngle = 23159;//14;


int main() {
    //! Defining structs
    MarioState *m = (MarioState *) malloc(sizeof(*m));
    Surface *floorList[numFlos];
    Surface *ceilingList[numCeils];
    Surface *floor;
    Surface *ceiling;
    Controller *controller;
    //surface = init_surface_data((s16 ***) **triList, 0);

    //! add tris to list

    *floList[0] = *flo1;// *triList[1] = *tri2;
    for (s16 i = 0; i < numFlos; i++) {

        //numSurfaces = sizeof vertexData / sizeof *vertexData;
        floorList[i] = init_surface_data((s16 ***) **floList, i);

        //surface->type = surfaceType;
    }
    for (s16 i = 0; i < numCeils; i++) {

        ceilingList[i] = init_surface_data( (s16 ***) **floList, i);

    }//m->floor = surface;


    //! Set mario's variables to starting
    m->pos[0] = mPos[0]; m->pos[1] = mPos[1]; m->pos[2] = mPos[2];
    m->faceAngle[1] = faceAngle;
    //! Testing if mario is in triangle
    //printf("%f\n", m->forwardVel);
    m->intendedMag = 0.0f;
    m->intendedYaw = 0;
    for ( int i = 0; i < frames; i++) {

        //m->controller->stickMag = 0.0f;
        //m->controller->stickX = 0.0f;
        //m->controller->stickY = 0.0f;


        //printf("%f\n", m->pos[0]);
        f32 floorHeight, ceilHeight;
        floorHeight = check_mario_floor(mPos, floorList, numFlos, &floor);
        ceilHeight = check_mario_ceil(mPos, ceilingList, numCeils, &ceiling);
        //printf("%i\n", ptInTriangle(m->pos, surface->vertex1, surface->vertex2, surface->vertex3));
        m->floor = floor;
        m->ceil = ceiling;
        //printf("%f\n", m->intendedMag);
        printf("%f\n", floorHeight);
        printf("%f\n", ceilHeight);
        //printf("%i\n", m->floor->vertex2[0]);
        //printf("%i\n", m->slideYaw);
        //update_sliding_angle(m, 2, 2);

        //update_sliding(m, 0);
        //printf("%f\n", m->pos[0]);

    }

}
