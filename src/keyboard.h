#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "define.h"
#include "render.h"

void EnneaKey(unsigned char key, int x, int y);
void EnneaKeyUp(unsigned char key, int x, int y);

void EnneaKey(unsigned char key, int x, int y)
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
                EnneaEnd();
                break;
        }

        Keyboard(key);
}

void EnneaKeyUp(unsigned char key, int x, int y)
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

#endif