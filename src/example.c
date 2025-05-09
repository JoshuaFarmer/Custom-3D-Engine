#define EnneaDrawGrid_ 1
#include "engine.h"

void Init(int argc, char *argv[])
{
        EnneaCreateObjects(1);
        Objects[0]->pos[0] = 10;
        Objects[0]->pos[1] = 1;
        Objects[0]->pos[2] = 10;
        Objects[0]->rvel[1] = 100.0f;
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