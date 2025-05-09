#define EnneaDrawGrid_ 1
#include "Ennea3D/Ennea.h"

void Init(int argc, char *argv[])
{
        EnneaCreateObjects(1);
        Objects[0]->pos[0] = 10;
        Objects[0]->pos[1] = 0;
        Objects[0]->pos[2] = 10;
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