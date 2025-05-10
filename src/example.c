#define EnneaDrawGrid_ 1
#include "Ennea3D/Ennea.h"

void Init(int argc, char *argv[])
{
        air_resistance = 0;
        EnneaCreateObjects(2);
        EnneaListObjects();

        Object *Guy = EnneaGetObject(1);
        Object *Monitor = EnneaGetObject(0);
        EnneaSetMesh(Monitor,"assets/monitor.obj");
        EnneaSetMesh(Guy,"assets/guy.obj");
        Monitor->texture = EnneaLoadTexture("assets/monitor.png");
        EnneaTorque(Monitor,0,1.0,0);
        EnneaTorque(Guy,0,15.0,0);
        EnneaPositionXYZ(Guy,10,10,10);
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