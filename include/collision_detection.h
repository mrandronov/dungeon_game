#ifndef __COLLISION_DETECTION_H__
#define __COLLISION_DETECTION_H__

#include "cglm/cglm.h"

#include "renderer.h"

typedef struct bounding_sphere_t
{
        vec3    pos;
        float   radius;

        bool    (*collides)(struct bounding_sphere_t* self, RenderObject* object);
} Bounding_Sphere;

// TODO: This number should eventually be in the 1000's.
#define  MAX_COLLIDABLE 256

typedef Bounding_Sphere**       Collidable_List;

Bounding_Sphere*                BoundingSphereCreate(float x, float y, float radius);

#endif /* __COLLISION_DETECTION_H__ */
