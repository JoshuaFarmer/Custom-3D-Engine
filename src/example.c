#include "engine.h"

void Init(int argc, char *argv[])
{
        _EngineCreateObjects(1);
        Objects[0]->pos[1] = -5;
}

void Draw()
{
        _EngineDrawObjects();
}

void End()
{
}

void DrawOnCamera()
{
}

void Keyboard(char key)
{
        return;
}