#ifndef ENGINE_H
#define ENGINE_H

#include "define.h"
#include "render.h"
#include "player.h"
#include "image.h"
#include "math.h"
#include "keyboard.h"

// --------------------- //

void Init(int argc, char *argv[]);

// --------------------- //

void EnneaInit();
void EnneaReshape(int w, int h);
void EnneaTimer();

// --------------------- //

void EnneaInit()
{
        glutSetCursor(GLUT_CURSOR_NONE);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        glutWarpPointer(width / 2, height / 2);
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
}

void EnneaEnd()
{
        EnneaDestroyObjects();
        free(Objects);
        End();
        exit(0);
}

void EnneaReshape(int w, int h)
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
void EnneaTimer()
{
        glutPostRedisplay();
        if (lock_cursor)
                glutWarpPointer(width / 2, height / 2);
        glutTimerFunc(1000 / FPS, EnneaTimer, 0);
}

void EnneaMouseClick(int button, int state, int x, int y)
{
        if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
        {
        }
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
        glutCreateWindow(EnneaWindowName);
        GLenum err = glewInit();
        if (err != GLEW_OK)
        {
                printf("Error: %s\n", glewGetErrorString(err));
                exit(1);
        }

        EnneaInit();
        Init(argc, argv);

        glutDisplayFunc(EnneaRender);
        glutKeyboardFunc(EnneaKey);
        glutKeyboardUpFunc(EnneaKeyUp);
        glutPassiveMotionFunc(EnneaMotion);
        glutMouseFunc(EnneaMouseClick);
        glutTimerFunc(0, EnneaTimer, 0);

        glRotatef(0, 0, 0, 0);

        glutMainLoop();
        EnneaEnd();
}

#endif