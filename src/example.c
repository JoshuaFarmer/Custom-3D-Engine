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
        EnneaGoto(Objects[0],0,5,0);
        EnneaGoto(Objects[1],5,5,5);
        EnneaGoto(Objects[2],-5,5,-5);
        EnneaGoto(Objects[3],-3,7,-3);
        EnneaTorque(Objects[0],0.0,25.0,0.0);
        EnneaTorque(Objects[1],5.0,2.0,1.0);
        EnneaTorque(Objects[2],5.0,2.0,1.0);
        EnneaTorque(Objects[3],5.0,2.0,1.0);
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