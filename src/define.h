#ifndef DEF_H
#define DEF_H

#define GLEW_STATIC
#define FAST_OBJ_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define TO_RADIANS 3.14 / 180.0
#define PI M_PI
#define FPS 60

#define ENGINE_VERSION 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include "imports/obj.h"
#include "imports/stb_image.h"

#ifndef _EngineDrawGrid_
#define _EngineDrawGrid_ 0
#endif
#ifndef _EngineWindowName
#define _EngineWindowName "engine"
#endif

typedef struct
{
        bool Forward, Backward, Left, Right, Up, Down;
} Motion;

typedef struct
{
        double minX, maxX;
        double minY, maxY;
        double minZ, maxZ;
} BoundingBox;

typedef struct
{
        // mesh & texture
        int texw, texh, nrchannels;
        GLuint texture;
        fastObjMesh *mesh;

        // properties
        double scale[3], pos[3], colour[3];
        double rot[3];
        double vel[3];
        double rvel[3];
        bool frozen;
        int ID;
        int GpInt[128];
        BoundingBox Collision;
        double radius;
} Object;

Object **Objects;
int object_count = 0;

double deltaTime = 0;
double width = 1920 / 2;
double height = 1080 / 2;
double grid_size = 100;
double grid_part_size = 1;
double mouseSensitivity = 0.5;
double global_gravity = -0.980;
double oldTimeSinceStart = 0;
double air_resistance = 0.5f;
bool lock_cursor = true, en_gravity = true, nerd = false;
bool fucked = false;

void _EngineEnd();
void Draw();
void End();

#endif