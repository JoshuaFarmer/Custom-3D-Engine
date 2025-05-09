#ifndef RENDER_H
#define RENDER_H

void _EngineStatsForNerds(Object *object);
void _EngineRender();
void _EngineDrawGrid();

#include "define.h"
#include "player.h"
#include "object.h"

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

        if (_EngineDrawGrid_)
        {
                _EngineDrawGrid();
        }
        /* update */
        for (int i = 0; i < object_count; ++i)
        {
                _EngineUpdateObject(Objects[i], deltaTime, air_resistance);
                handlePlayerCollision(Objects[i]);
        }

        Draw();
        _EnginePlayerGravity();

        int timeSinceStart = glutGet(GLUT_ELAPSED_TIME);
        deltaTime = timeSinceStart - oldTimeSinceStart;
        oldTimeSinceStart = timeSinceStart;
        glutSwapBuffers();
}

#endif