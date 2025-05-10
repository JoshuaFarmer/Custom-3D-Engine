#ifndef OBJ_H
#define OBJ_H

#include "define.h"
#include "render.h"

GLuint EnneaLoadTexture(char *path);
void EnneaUpdateObject(Object *, double, double);
void EnneaDrawObject(Object *object);
Object *EnneaCreateObject(char *path);
void EnneaDrawObject(Object *object);
void EnneaSetMesh(Object *obj, char *path);
void EnneaCreateObjects(int count);
void EnneaDestroyObjects();
void EnneaDestroyObject(Object *object);
void EnneaListObjects();
void EnneaAddObject(Object *obj);

void EnneaDestroyObject(Object *object)
{
        fast_obj_destroy(object->mesh);
        free(object);
}

void EnneaDestroyObjects()
{
        Object *obj = RootObject;
        Object *prv = NULL;
        while (obj)
        {
                if (prv)
                        EnneaDestroyObject(prv);
                prv = obj;
                obj = obj->next;
        }
}

void EnneaCreateObjects(int count)
{
        EnneaDestroyObjects();
        object_count = count;
        for (int i = 0; i < count; ++i)
        {
                EnneaCreateObject("assets/box.obj");
        }
}

void EnneaSetMesh(Object *obj, char *path)
{
        fast_obj_destroy(obj->mesh);
        obj->mesh = fast_obj_read(path);
}

Object *EnneaCreateObject(char *path)
{
        static int ID=0;
        Object *object = malloc(sizeof(Object));
        object->mesh = fast_obj_read(path);
        // default values
        object->scale[0] = 1.0f;
        object->scale[1] = 1.0f;
        object->scale[2] = 1.0f;
        object->ID = ID++;

        object->Collision.minX = -(object->scale[0] / 2);
        object->Collision.maxX = object->scale[0] / 2;

        object->Collision.minY = -(object->scale[1] / 2);
        object->Collision.maxY = object->scale[1] / 2;

        object->Collision.minZ = (-object->scale[2]) / 2;
        object->Collision.maxZ = object->scale[2] / 2;

        object->pos[0] = 0.0f;
        object->pos[1] = 0.0f;
        object->pos[2] = 0.0f;
        object->colour[0] = 1.0f;
        object->colour[1] = 1.0f;
        object->colour[2] = 1.0f;
        object->rot[0] = 0.0f;
        object->rot[1] = 0.0f;
        object->rot[2] = 0.0f;

        object->vel[0] = 0.0f;
        object->vel[1] = 0.0f;
        object->vel[2] = 0.0f;

        object->rvel[0] = 0.0f;
        object->rvel[1] = 0.0f;
        object->rvel[2] = 0.0f;

        object->frozen = true;
        object->texture = EnneaLoadTexture("assets/no-texture.png");
        EnneaAddObject(object);
        return object;
}

void EnneaDrawObject(Object *object)
{
        glPushMatrix();
        glTranslatef(object->pos[0], object->pos[1], object->pos[2]);
        glRotatef(object->rot[0], 1, 0, 0);
        glRotatef(object->rot[1], 0, 1, 0);
        glRotatef(object->rot[2], 0, 0, 1);
        glScalef(object->scale[0], object->scale[1], object->scale[2]);
        glColor3f(object->colour[0], object->colour[1], object->colour[2]);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, object->texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        for (unsigned int i = 0; i < object->mesh->face_count; ++i)
        {
                glBegin(GL_POLYGON);
                for (unsigned int j = 0; j < object->mesh->face_vertices[i]; ++j)
                {
                        fastObjIndex index = object->mesh->indices[i * 3 + j];

                        glTexCoord2f(
                            object->mesh->texcoords[index.t * 2],
                            1.0 - object->mesh->texcoords[index.t * 2 + 1]);

                        glVertex3f(
                            object->mesh->positions[index.p * 3] * object->scale[0],
                            object->mesh->positions[index.p * 3 + 1] * object->scale[1],
                            object->mesh->positions[index.p * 3 + 2] * object->scale[2]);
                }
                glEnd();
        }

        // Disable texturing
        glDisable(GL_TEXTURE_2D);

        // Reset the color to white
        glColor3f(1, 1, 1);

        glPopMatrix();
}

void EnneaUpdateObject(Object *obj, double deltaTime, double air_resistance)
{
        obj->rot[0] += (obj->rvel[0] / 25) * deltaTime;
        obj->rot[1] += (obj->rvel[1] / 25) * deltaTime;
        obj->rot[2] += (obj->rvel[2] / 25) * deltaTime;

        obj->pos[0] += (obj->vel[0] / 25) * deltaTime;
        obj->pos[1] += (obj->vel[1] / 25) * deltaTime;
        obj->pos[2] += (obj->vel[2] / 25) * deltaTime;

        // Handle air resistance
        for (int i = 0; i < 3; ++i)
        {
                if (obj->vel[i] > 0)
                        obj->vel[i] -= air_resistance;
                else if (obj->vel[i] < 0)
                        obj->vel[i] += air_resistance;

                if (obj->rvel[i] > 0)
                        obj->rvel[i] -= air_resistance;
                else if (obj->rvel[i] < 0)
                        obj->rvel[i] += air_resistance;
        }
}

int EnneaCheckCollision(Object *obj1, Object *obj2)
{
        // object i
        obj1->Collision.minX = -obj1->scale[0];
        obj1->Collision.maxX = obj1->scale[0];

        obj1->Collision.minY = -obj1->scale[1];
        obj1->Collision.maxY = obj1->scale[1];

        obj1->Collision.minZ = -obj1->scale[2];
        obj1->Collision.maxZ = obj1->scale[2];

        // object ii
        obj2->Collision.minX = -obj2->scale[0];
        obj2->Collision.maxX = obj2->scale[0];

        obj2->Collision.minY = -obj2->scale[1];
        obj2->Collision.maxY = obj2->scale[1];

        obj2->Collision.minZ = -obj2->scale[2];
        obj2->Collision.maxZ = obj2->scale[2];

        // Check for overlap along each axis
        if ((obj1->Collision.maxX - obj1->pos[0]) < (obj2->Collision.minX - obj2->pos[0]) || (obj1->Collision.minX - obj1->pos[0]) > (obj2->Collision.maxX - obj2->pos[0]))
                return 0; // No collision along X
        if ((obj1->Collision.maxY - obj1->pos[1]) < (obj2->Collision.minY - obj2->pos[1]) || (obj1->Collision.minY - obj1->pos[1]) > (obj2->Collision.maxY - obj2->pos[1]))
                return 0; // No collision along Y
        if ((obj1->Collision.maxZ - obj1->pos[2]) < (obj2->Collision.minZ - obj2->pos[2]) || (obj1->Collision.minZ - obj1->pos[2]) > (obj2->Collision.maxZ - obj2->pos[2]))
                return 0; // No collision along Z
        return 1;         // Collided along all axes
}

void EnneaDrawObjects()
{
        Object *obj = RootObject;
        while (obj)
        {
                EnneaDrawObject(obj);
                if (nerd)
                        EnneaStatsForNerds(obj);
                obj = obj->next;
        }
}

void EnneaPositionXYZ(Object *obj, double x, double y, double z)
{
        obj->vel[0] = 0;
        obj->vel[1] = 0;
        obj->vel[2] = 0;
        obj->pos[0] = x;
        obj->pos[1] = y;
        obj->pos[2] = z;
}

void EnneaRotateXYZ(Object *obj, double x, double y, double z)
{
        obj->rvel[0] = 0;
        obj->rvel[1] = 0;
        obj->rvel[2] = 0;
        obj->rot[0] = x;
        obj->rot[1] = y;
        obj->rot[2] = z;
}

void EnneaTorque(Object *obj, double x, double y, double z)
{
        obj->rvel[0] += x;
        obj->rvel[1] += y;
        obj->rvel[2] += z;
}

void EnneaForceXYZ(Object *obj, double x, double y, double z)
{
        obj->vel[0] += x;
        obj->vel[1] += y;
        obj->vel[2] += z;
}

void EnneaForce(Object *obj, double force)
{
        if (obj->frozen)
                return;

        double pitch = obj->rot[0];
        double yaw = obj->rot[1];
        double fx = cos(pitch) * sin(yaw);
        double fy = sin(pitch);
        double fz = cos(pitch) * cos(yaw);
        printf("x=%lf,y=%lf,z=%lf\n",fx,fy,fz);
        obj->vel[0] += fx * force;
        obj->vel[1] += fy * force;
        obj->vel[2] += fz * force;
}

void EnneaAddObject(Object *obj)
{
        if (!obj) return;
        if (!RootObject)
        {
                RootObject = obj;
                RootObject->next = NULL;
                return;
        }

        Object *end = RootObject;
        while (end->next)
        {
                end = end->next;
        }
        end->next = obj;
        obj->next = NULL;
}

void EnneaListObjects()
{
        Object *obj = RootObject;
        while (obj)
        {
                printf("Object @%p,ID=%d\n",obj,obj->ID);
                obj = obj->next;
        }
}

Object *EnneaGetObject(int index)
{
        Object *obj = RootObject;
        while (index--)
        {
                if (obj == NULL) return NULL;
                obj = obj->next;
        }

        return obj;
}

#endif
