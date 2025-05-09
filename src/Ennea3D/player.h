/* for all player/camera related functions */
#ifndef PLAYER_H
#define PLAYER_H

#include "define.h"

bool EnneaPlayerColliding();
void EnneaPlayerGravity();
void EnneaHandlePlayerCollision(Object *obj);
void EnneaCamera();
void EnneaMotion(int x, int y);
void EnneaMove(char code);

Motion motion = {false, false, false, false, false, false};

double pitch = 0.0, yaw = 0.0, roll = 0.0;
double camX = 0, camY = 0, camZ = 0;
double camYVel = 0;
double player_height = 2;
double GRAVITY = 1;
double camSpeedIncrease = 1;
bool camera = false;

void EnneaMove(char code)
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
                camY += (0.1 / 25) * deltaTime;
                break;
        case 'Q':
                camY -= (0.1 / 25) * deltaTime;
                break;
        }
}

void EnneaMotion(int x, int y)
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

void EnneaCamera()
{
        /* movement */
        if (motion.Forward)
                EnneaMove('W');
        if (motion.Backward)
                EnneaMove('S');
        if (motion.Left)
                EnneaMove('A');
        if (motion.Right)
                EnneaMove('D');
        if (motion.Up)
                EnneaMove('E');
        if (motion.Down)
                EnneaMove('Q');
        pitch = (pitch >= 90) ? 90 : pitch;
        pitch = (pitch <= -90) ? -90 : pitch;
        camera = true;
        DrawOnCamera();
        camera = false;
        glRotatef(-pitch, 1.0, 0.0, 0.0);
        glRotatef(-yaw, 0.0, 1.0, 0.0);
        glRotatef(-roll, 0.0, 0.0, 1.0);
        camY += camYVel * deltaTime;
        while (EnneaPlayerColliding())
                camY += 0.01;
        glTranslatef(-camX, -camY, -camZ);
}

// Function to check collision between player and object
bool EnneaPlayerObjectCollision(const Object *obj)
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

void EnneaHandlePlayerCollision(Object *obj)
{
        // Adjust the player's position based on the collision
        if (EnneaPlayerObjectCollision(obj))
        {
                // Example of simple collision response
                // This can be more sophisticated based on your game design
                // Here we just set the player's position to be on the object's surface
                camY = obj->Collision.maxY + player_height; // Position above the object
                camYVel = 0;                                // Stop downward velocity
        }
}

bool EnneaPlayerColliding()
{
        int collisions = 0;
        for (int a = 0; a < object_count; ++a)
        {
                collisions += EnneaPlayerObjectCollision(Objects[a]);
        }
        return collisions != 0;
}

void EnneaPlayerGravity()
{
        if (EnneaPlayerColliding() == false)
                camYVel -= (GRAVITY / 1000000) * deltaTime;
}

#endif