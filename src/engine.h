#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

#define GLEW_STATIC
#include <GL/glew.h>

#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>

#define FAST_OBJ_IMPLEMENTATION
#include "obj.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define TO_RADIANS 3.14 / 180.0

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
        double vel[0];
        double rvel[0];
        bool frozen;
        int ID;
        int GpInt[128];
        BoundingBox Collision;
        double radius;
} Object;

double width = 1920 / 2;
double height = 1080 / 2;

bool lock_cursor = true, en_gravity = true, nerd = false;
bool fucked = false;

double pitch = 0.0, yaw = 0.0, roll = 0.0;
double camX = 0, camY = 0, camZ = 0;
double camYVel = 0;
double player_height = 2;
double GRAVITY = 1;

double
    grid_size = 100,
    grid_part_size = 1,
    camSpeedIncrease = 1,
    mouseSensitivity = 0.5,
    global_gravity = -0.980,
    oldTimeSinceStart = 0,
    deltaTime = 0;

double air_resistance = 0.5f;
bool camera = false;

// --------------------- //

Motion motion = {false, false, false, false, false, false};

int object_count = 0;

Object **Objects;

// --------------------------------------- //

GLuint _EngineLoadTexture(char *path);
double _Distance(double camx, double camy, double camz, double x, double y, double z);
void _EngineDrawObject(Object *object);
Object *_EngineCreateObject(char *path);
void _EngineDestroyObject(Object *object);
void _EngineDestroyObjects();
void _EngineCreateObjects(int count);
void _EngineStatsForNerds(Object *object);
void _EngineDrawObjects();
void _EngineSetMesh(Object *obj, char *path);
void handlePlayerCollision(Object *obj);

// --------------------------------------- //

GLuint _EngineLoadTexture(char *path)
{
        int width, height, channels;
        unsigned char *data = stbi_load(path, &width, &height, &channels, 0);
        if (data == NULL)
        {
                printf("Failed to load image.\n");
                return 0;
        }
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);
        return texture;
}

double _Distance(double camx, double camy, double camz, double x, double y, double z)
{
        return sqrt(((camx - x) * (camx - x)) + ((camy - y) * (camy - y)) + ((camz - z) * (camz - z)));
}

void _EngineDrawObject(Object *object)
{
        glPushMatrix();
        if (camera == false)
        {
                glRotatef(object->rot[0], 1, 0, 0);
                glRotatef(object->rot[1], 0, 1, 0);
                glRotatef(object->rot[2], 0, 0, 1);
        }

        glColor3f(object->colour[0], object->colour[1], object->colour[2]);

        // Enable 2D texturing
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, object->texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        for (unsigned int i = 0; i < object->mesh->face_count; ++i)
        {
                glBegin(GL_POLYGON);
                for (unsigned int j = 0; j < object->mesh->face_vertices[i]; ++j)
                {
                        fastObjIndex index = object->mesh->indices[i * 3 + j];

                        // Add texture coordinates
                        glTexCoord2f(
                            object->mesh->texcoords[index.t * 2],
                            1.0 - object->mesh->texcoords[index.t * 2 + 1]);

                        glVertex3f(
                            object->pos[0] + object->mesh->positions[index.p * 3] * object->scale[0],
                            object->pos[1] + object->mesh->positions[index.p * 3 + 1] * object->scale[1],
                            object->pos[2] + object->mesh->positions[index.p * 3 + 2] * object->scale[2]);
                }
                glEnd();
        }

        glDisable(GL_TEXTURE_2D);
        glPopMatrix();
        glColor3f(1, 1, 1);
}

Object *_EngineCreateObject(char *path)
{
        Object *object = malloc(sizeof(Object));
        object->mesh = fast_obj_read(path);
        // default values

        object->scale[0] = 1.0f;
        object->scale[1] = 1.0f;
        object->scale[2] = 1.0f;

        object->Collision.minX = -object->scale[0];
        object->Collision.maxX = object->scale[0];

        object->Collision.minY = -object->scale[1];
        object->Collision.maxY = object->scale[1];

        object->Collision.minZ = -object->scale[2];
        object->Collision.maxZ = object->scale[2];

        object->pos[0] = 0.0f;
        object->pos[1] = 0.0f;
        object->pos[2] = 0.0f;
        object->colour[0] = 1.0f;
        object->colour[1] = 1.0f;
        object->colour[2] = 1.0f;
        object->rot[0] = 0.0f;
        object->rot[1] = 0.0f;
        object->rot[2] = 0.0f;

        object->vel[0] = 0.0f;
        object->vel[1] = 0.0f;
        object->vel[2] = 0.0f;

        object->rvel[0] = 0.0f;
        object->rvel[1] = 0.0f;
        object->rvel[2] = 0.0f;

        object->frozen = true;
        object->texture = _EngineLoadTexture("assets/no-texture.png");

        return object;
}

void _EngineDestroyObject(Object *object)
{
        fast_obj_destroy(object->mesh);
        free(object);
}

void _EngineDestroyObjects()
{
        for (int i = 0; i < object_count; ++i)
                _EngineDestroyObject(Objects[i]);
        free(Objects);
}

void _EngineCreateObjects(int count)
{
        for (int i = 0; i < object_count; ++i)
                _EngineDestroyObject(Objects[i]);
        free(Objects);
        Objects = malloc(count * sizeof(Object *));
        object_count = count;
        for (int i = 0; i < count; ++i)
        {
                Objects[i] = _EngineCreateObject("assets/box.obj");
        }
}

void _EngineStatsForNerds(Object *object)
{
        glPushMatrix();
        glRotatef(object->rot[0], 1, 0, 0);
        glRotatef(object->rot[1], 0, 1, 0);
        glRotatef(object->rot[2], 0, 0, 1);

        glColor3f(1 - object->colour[0], 1 - object->colour[1], 1 - object->colour[2]);
        glBegin(GL_LINES);
        glVertex3f(object->pos[0], object->pos[1] - 10, object->pos[2]);
        glVertex3f(object->pos[0], object->pos[1] + 10, object->pos[2]);
        glEnd();

        glBegin(GL_LINES);
        glVertex3f(object->pos[0] - 10, object->pos[1], object->pos[2]);
        glVertex3f(object->pos[0] + 10, object->pos[1], object->pos[2]);
        glEnd();

        glBegin(GL_LINES);
        glVertex3f(object->pos[0], object->pos[1], object->pos[2] - 10);
        glVertex3f(object->pos[0], object->pos[1], object->pos[2] + 10);
        glEnd();

        glPopMatrix();
}

void _EngineDrawObjects()
{
        for (int i = 0; i < object_count; ++i)
        {
                Objects[i]->rot[0] += (Objects[i]->rvel[0] / 25) * deltaTime;
                Objects[i]->rot[1] += (Objects[i]->rvel[1] / 25) * deltaTime;
                Objects[i]->rot[2] += (Objects[i]->rvel[2] / 25) * deltaTime;
                Objects[i]->pos[0] += (Objects[i]->vel[0] / 25) * deltaTime;
                Objects[i]->pos[1] += (Objects[i]->vel[1] / 25) * deltaTime;
                Objects[i]->pos[2] += (Objects[i]->vel[2] / 25) * deltaTime;

                _EngineDrawObject(Objects[i]);
                if (nerd)
                        _EngineStatsForNerds(Objects[i]);
        }
}

void _EngineSetMesh(Object *obj, char *path)
{
        fast_obj_destroy(obj->mesh);
        obj->mesh = fast_obj_read(path);
}

// Function to check collision between player and object
bool checkPlayerObjectCollision(const Object *obj)
{
        // Calculate the player's new position based on its velocity
        double newCamY = camY + camYVel * deltaTime;

        // Check if the player's bounding box intersects with the object's bounding box
        bool collisionX = (camX + player_height) >= (obj->Collision.minX + obj->pos[0]) &&
                          (camX - player_height) <= (obj->Collision.maxX + obj->pos[0]);

        bool collisionY = (newCamY + player_height) >= (obj->Collision.minY + obj->pos[1]) &&
                          (newCamY - player_height) <= (obj->Collision.maxY + obj->pos[1]);

        bool collisionZ = (camZ + player_height) >= (obj->Collision.minZ + obj->pos[2]) &&
                          (camZ - player_height) <= (obj->Collision.maxZ + obj->pos[2]);

        return collisionX && collisionY && collisionZ;
}

#define PI M_PI
#define FPS 60

#ifndef _EngineDrawGrid_
#define _EngineDrawGrid_ 0
#endif

#ifndef _EngineWindowName
#define _EngineWindowName "engine"
#endif

// --- functions --- //

void _EngineInit();
void _EngineRender();
void _EngineEnd();
void _EngineDrawGrid();
void _EngineReshape(int w, int h);
void _EngineTimer();
void _EngineMotion(int x, int y);
void _EngineCamera();
void _EngineKey(unsigned char key, int x, int y);
void _EngineKeyUp(unsigned char key, int x, int y);
void _EngineMove(char code);
bool _EnginePlayerColliding();
void _EnginePlayerGravity();

void Draw();
void Init(int argc, char *argv[]);
void Keyboard(char key);
void DrawOnCamera();
void End();

// ------------------------------------- //

void _EngineInit()
{
        glutSetCursor(GLUT_CURSOR_NONE);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        glutWarpPointer(width / 2, height / 2);
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
}

void _EngineUpdateObject(Object *obj)
{
        obj->rot[0] += (obj->rvel[0] / 25) * deltaTime;
        obj->rot[1] += (obj->rvel[1] / 25) * deltaTime;
        obj->rot[2] += (obj->rvel[2] / 25) * deltaTime;

        obj->pos[0] += (obj->vel[0] / 25) * deltaTime;
        obj->pos[1] += (obj->vel[1] / 25) * deltaTime;
        obj->pos[2] += (obj->vel[2] / 25) * deltaTime;

        // Handle air resistance
        for (int i = 0; i < 3; ++i)
        {
                if (obj->vel[i] > 0)
                        obj->vel[i] -= air_resistance;
                else if (obj->vel[i] < 0)
                        obj->vel[i] += air_resistance;

                if (obj->rvel[i] > 0)
                        obj->rvel[i] -= air_resistance;
                else if (obj->rvel[i] < 0)
                        obj->rvel[i] += air_resistance;
        }
}

void _EngineRender()
{
        // clear
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(80, width / height, 0.1, 10000);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        _EngineCamera();

        for (int i = 0; i < object_count; ++i)
        {
                _EngineUpdateObject(Objects[i]);

                // Check collision with the player
                handlePlayerCollision(Objects[i]);

                _EngineDrawObject(Objects[i]);
                if (nerd)
                        _EngineStatsForNerds(Objects[i]);
        }

        _EnginePlayerGravity();

        int timeSinceStart = glutGet(GLUT_ELAPSED_TIME);
        deltaTime = timeSinceStart - oldTimeSinceStart;
        oldTimeSinceStart = timeSinceStart;
        glutSwapBuffers();
}

int main(int argc, char *argv[])
{
        // init GL
        glutInit(&argc, argv);
        // set display mode
        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
        // start position, dimensions, title
        glutInitWindowPosition(100, 100);
        glutInitWindowSize(width, height);
        glutCreateWindow(_EngineWindowName);
        GLenum err = glewInit();
        if (err != GLEW_OK)
        {
                printf("Error: %s\n", glewGetErrorString(err));
                exit(1);
        }

        _EngineInit();
        Init(argc, argv);

        glutDisplayFunc(_EngineRender);
        glutKeyboardFunc(_EngineKey);
        glutKeyboardUpFunc(_EngineKeyUp);
        glutPassiveMotionFunc(_EngineMotion);
        glutTimerFunc(0, _EngineTimer, 0);

        glRotatef(0, 0, 0, 0);

        glutMainLoop();
        _EngineEnd();
}

void _EngineEnd()
{
        _EngineDestroyObjects();
        free(Objects);
        End();
        exit(0);
}

void _EngineDrawGrid()
{
        glColor3f(0, 0, 0);
        // draw x lines
        for (double z = 0; z <= grid_size * 2; z = z + grid_part_size)
        {
                glBegin(GL_LINES);
                glVertex3f(-grid_size, 0, z - grid_size);
                glVertex3f(grid_size, 0, z - grid_size);
                glEnd();
        }
        // draw z lines
        for (double x = 0; x <= grid_size * 2; x = x + grid_part_size)
        {
                glBegin(GL_LINES);
                glVertex3f(x - grid_size, 0, -grid_size);
                glVertex3f(x - grid_size, 0, grid_size);
                glEnd();
        }

        glColor3f(1, 0, 0);
        glBegin(GL_LINES);
        glVertex3f(-grid_size, 0, 0);
        glVertex3f(grid_size, 0, 0);
        glEnd();

        glColor3f(0, 1, 0);
        glBegin(GL_LINES);
        glVertex3f(0, -grid_size, 0);
        glVertex3f(0, grid_size, 0);
        glEnd();

        glColor3f(0, 0, 1);
        glBegin(GL_LINES);
        glVertex3f(0, 0, -grid_size);
        glVertex3f(0, 0, grid_size);
        glEnd();

        glColor3f(1, 1, 1);
}

void _EngineReshape(int w, int h)
{
        glViewport(0, 0, w, h);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(80, 16.0 / 9.0, 0.001, 3000);
        glMatrixMode(GL_MODELVIEW);
}

/*
  this funtion is used to keep calling the display function periodically
  at a rate of FPS times in one second. The constant FPS is defined above and
  has the value of 60
*/
void _EngineTimer()
{
        glutPostRedisplay();
        if (lock_cursor)
                glutWarpPointer(width / 2, height / 2);
        glutTimerFunc(1000 / FPS, _EngineTimer, 0);
}

void _EngineMotion(int x, int y)
{
        /*
                two variables to store X and Y coordinates, as observed from the center
                of the window
        */

        int dev_x, dev_y;
        dev_x = (width / 2) - x;
        dev_y = (height / 2) - y;
        /* apply the changes to pitch and yaw*/
        yaw += ((((double)dev_x / 20.0) * mouseSensitivity) / 25) * deltaTime;
        pitch += ((((double)dev_y / 20.0) * mouseSensitivity) / 25) * deltaTime;
}

void _EngineMove(char code)
{
        switch (code)
        {
        case 'W':
                camX += (((cos((yaw + 90) * TO_RADIANS) / 5.0) * camSpeedIncrease) / 25) * deltaTime;
                camZ -= (((sin((yaw + 90) * TO_RADIANS) / 5.0) * camSpeedIncrease) / 25) * deltaTime;
                break;
        case 'S':
                camX += (((cos((yaw + 90 + 180) * TO_RADIANS) / 5.0) * camSpeedIncrease) / 25) * deltaTime;
                camZ -= (((sin((yaw + 90 + 180) * TO_RADIANS) / 5.0) * camSpeedIncrease) / 25) * deltaTime;
                break;
        case 'A':
                camX += (((cos((yaw + 90 + 90) * TO_RADIANS) / 5.0) * camSpeedIncrease) / 25) * deltaTime;
                camZ -= (((sin((yaw + 90 + 90) * TO_RADIANS) / 5.0) * camSpeedIncrease) / 25) * deltaTime;
                break;
        case 'D':
                camX += (((cos((yaw + 90 - 90) * TO_RADIANS) / 5.0) * camSpeedIncrease) / 25) * deltaTime;
                camZ -= (((sin((yaw + 90 - 90) * TO_RADIANS) / 5.0) * camSpeedIncrease) / 25) * deltaTime;
                break;
        case 'E':
                camY += 0.2;
                break;
        case 'Q':
                camY -= 0.2;
                break;
        }
}

void _EngineCamera()
{
        if (motion.Forward)
        {
                camX += (((cos((yaw + 90) * TO_RADIANS) / 5.0) * camSpeedIncrease) / 25) * deltaTime;
                camZ -= (((sin((yaw + 90) * TO_RADIANS) / 5.0) * camSpeedIncrease) / 25) * deltaTime;
        }
        if (motion.Backward)
        {
                camX += (((cos((yaw + 90 + 180) * TO_RADIANS) / 5.0) * camSpeedIncrease) / 25) * deltaTime;
                camZ -= (((sin((yaw + 90 + 180) * TO_RADIANS) / 5.0) * camSpeedIncrease) / 25) * deltaTime;
        }
        if (motion.Left)
        {
                camX += (((cos((yaw + 90 + 90) * TO_RADIANS) / 5.0) * camSpeedIncrease) / 25) * deltaTime;
                camZ -= (((sin((yaw + 90 + 90) * TO_RADIANS) / 5.0) * camSpeedIncrease) / 25) * deltaTime;
        }
        if (motion.Right)
        {
                camX += (((cos((yaw + 90 - 90) * TO_RADIANS) / 5.0) * camSpeedIncrease) / 25) * deltaTime;
                camZ -= (((sin((yaw + 90 - 90) * TO_RADIANS) / 5.0) * camSpeedIncrease) / 25) * deltaTime;
        }
        if (motion.Up)
        {
                camY += (0.1 / 25) * deltaTime;
        }
        if (motion.Down)
        {
                camY -= (0.1 / 25) * deltaTime;
        }
        /*
                limit the values of pitch
                between -90 and 90
        */
        if (pitch >= 90)
                pitch = 90;
        if (pitch <= -90)
                pitch = -90;

        camera = true;
        DrawOnCamera();
        camera = false;
        glRotatef(-pitch, 1.0, 0.0, 0.0);
        glRotatef(-yaw, 0.0, 1.0, 0.0);
        glRotatef(-roll, 0.0, 0.0, 1.0);
        camY += camYVel * deltaTime;
        while (_EnginePlayerColliding())
                camY += 0.01;
        glTranslatef(-camX, -camY, -camZ);
}

void _EngineKey(unsigned char key, int x, int y)
{
        switch (key)
        {
        case 'W':
        case 'w':
                motion.Forward = true;
                break;
        case 'A':
        case 'a':
                motion.Left = true;
                break;
        case 'S':
        case 's':
                motion.Backward = true;
                break;
        case 'D':
        case 'd':
                motion.Right = true;
                break;
        case 'E':
        case 'e':
                motion.Up = true;
                break;
        case 'Q':
        case 'q':
                motion.Down = true;
                break;
        case '+':
                grid_part_size = grid_part_size * 2;
                break;
        case '-':
                grid_part_size = grid_part_size / 2;
                break;
        case '!':
                nerd = !nerd;
                break;
        case 27:
                _EngineEnd();
                break;
        }

        {
                _EngineMove('W');
                if (_EnginePlayerColliding())
                {
                        motion.Forward = false;
                }
        }

        // Move backward
        {
                _EngineMove('S');
                if (_EnginePlayerColliding())
                {
                        // Move forward
                        motion.Backward = false;
                        _EngineMove('W');
                }
        }

        // Move right
        {
                _EngineMove('D');
                if (_EnginePlayerColliding())
                {
                        motion.Right = false;
                        _EngineMove('A');
                }
        }

        // Move left
        {
                _EngineMove('A');
                if (_EnginePlayerColliding())
                {
                        motion.Left = false;
                        _EngineMove('D');
                }
        }

        // Move up
        {
                _EngineMove('E');
                if (_EnginePlayerColliding())
                {
                        motion.Up = false;
                        _EngineMove('Q');
                }
        }

        // Move down
        {
                _EngineMove('Q');
                if (_EnginePlayerColliding())
                {
                        motion.Down = false;
                        _EngineMove('E');
                }
        }

        Keyboard(key);
}

void _EngineKeyUp(unsigned char key, int x, int y)
{
        switch (key)
        {
        case 'W':
        case 'w':
                motion.Forward = false;
                break;
        case 'A':
        case 'a':
                motion.Left = false;
                break;
        case 'S':
        case 's':
                motion.Backward = false;
                break;
        case 'D':
        case 'd':
                motion.Right = false;
                break;
        case 'E':
        case 'e':
                motion.Up = false;
                break;
        case 'Q':
        case 'q':
                motion.Down = false;
                break;
        }
}

int checkCollision(Object *obj1, Object *obj2)
{
        // object i
        obj1->Collision.minX = -obj1->scale[0];
        obj1->Collision.maxX = obj1->scale[0];

        obj1->Collision.minY = -obj1->scale[1];
        obj1->Collision.maxY = obj1->scale[1];

        obj1->Collision.minZ = -obj1->scale[2];
        obj1->Collision.maxZ = obj1->scale[2];

        // object ii
        obj2->Collision.minX = -obj2->scale[0];
        obj2->Collision.maxX = obj2->scale[0];

        obj2->Collision.minY = -obj2->scale[1];
        obj2->Collision.maxY = obj2->scale[1];

        obj2->Collision.minZ = -obj2->scale[2];
        obj2->Collision.maxZ = obj2->scale[2];

        // Check for overlap along each axis
        if ((obj1->Collision.maxX - obj1->pos[0]) < (obj2->Collision.minX - obj2->pos[0]) || (obj1->Collision.minX - obj1->pos[0]) > (obj2->Collision.maxX - obj2->pos[0]))
                return 0; // No collision along X
        if ((obj1->Collision.maxY - obj1->pos[1]) < (obj2->Collision.minY - obj2->pos[1]) || (obj1->Collision.minY - obj1->pos[1]) > (obj2->Collision.maxY - obj2->pos[1]))
                return 0; // No collision along Y
        if ((obj1->Collision.maxZ - obj1->pos[2]) < (obj2->Collision.minZ - obj2->pos[2]) || (obj1->Collision.minZ - obj1->pos[2]) > (obj2->Collision.maxZ - obj2->pos[2]))
                return 0; // No collision along Z
        return 1;         // Collided along all axes
}

void handlePlayerCollision(Object *obj)
{
        // Adjust the player's position based on the collision
        if (checkPlayerObjectCollision(obj))
        {
                // Example of simple collision response
                // This can be more sophisticated based on your game design
                // Here we just set the player's position to be on the object's surface
                camY = obj->Collision.maxY + player_height; // Position above the object
                camYVel = 0;                                // Stop downward velocity
        }
}

bool _EnginePlayerColliding()
{
        int collision = 0;
        for (int a = 0; a < object_count; ++a)
        {
                collision += checkPlayerObjectCollision(Objects[a]);
        }
        if (collision != 0)
        {
                return true;
        }
        return false;
}

void _EnginePlayerGravity()
{
        if (_EnginePlayerColliding() == false)
                camYVel -= (GRAVITY / 1000000) * deltaTime;
}
