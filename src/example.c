#define EnneaDrawGrid_ 1
#include "Ennea3D/Ennea.h"

void Init(int argc, char *argv[])
{
        air_resistance = 0;        
        EnneaCreateObjects(4);
        EnneaSetMesh(Objects[0],"assets/guy.obj");
        EnneaSetMesh(Objects[3],"assets/old computer monitor.obj");
        Objects[1]->texture = EnneaLoadTexture("assets/face.jpg");
        Objects[2]->texture = EnneaLoadTexture("assets/face.jpg");
        Objects[3]->texture = EnneaLoadTexture("assets/old computer monitor.png");
        Objects[0]->pos[0] = 0;
        Objects[0]->pos[1] = 5;
        Objects[0]->pos[2] = 0;
        Objects[0]->rvel[0] += 5.0f;
        Objects[0]->rvel[1] += 2.0f;
        Objects[0]->rvel[2] += 1.0f;

        Objects[1]->pos[0] = 5;
        Objects[1]->pos[1] = 5;
        Objects[1]->pos[2] = 5;
        Objects[1]->rvel[1] += 25.0f;
        Objects[2]->pos[0] = -5;
        Objects[2]->pos[1] = 5;
        Objects[2]->pos[2] = -5;
        Objects[2]->rvel[1] += 25.0f;

        Objects[3]->pos[0] = -3;
        Objects[3]->pos[1] = 7;
        Objects[3]->pos[2] = -3;
        Objects[3]->rvel[1] += 25.0f;
}

void Draw()
{
        EnneaDrawObjects();
}

void End()
{
}

void DrawOnCamera()
{
}

void Keyboard(char key)
{
}