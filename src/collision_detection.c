#include <stdlib.h>
#include <string.h>

#include "collision_detection.h"

bool
BoundingSphereCollides(Bounding_Sphere* self, RenderObject* object)
{
        float distance = glm_vec3_distance(self->pos, object->pos);

        if (distance > self->radius)
        {
                return false;
        }

        return true;
}

Bounding_Sphere*
BoundingSphereCreate(float x, float y, float radius)
{
        Bounding_Sphere* self = malloc(sizeof(Bounding_Sphere));

        memcpy(self->pos, Position(x, y, 0.0f), sizeof(vec3));
        self->radius = radius;

        self->collides = BoundingSphereCollides;

        return self;
}

