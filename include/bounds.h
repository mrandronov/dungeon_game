#ifndef __BOUNDS_H__
#define __BOUNDS_H__

#include "cglm/types.h"

#include "renderer.h"

typedef struct bounds_t
{
        RenderObject*   object;

        vec4            color;
        vec3            pos;
        
        void            (*destroy)(struct bounds_t* self);
} Bounds;

Bounds*                 BoundsCreate(ShaderProgram* shader, vec4 color, vec3 pos);

#endif /* __BOUNDS_H__ */
