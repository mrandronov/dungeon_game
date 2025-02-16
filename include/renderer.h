#ifndef __RENDERER_H__
#define __RENDERER_H__

#include "cglm/types.h"

#include "shader.h"
#include "texture.h"

typedef struct render_object_t
{
        char*           id;

        unsigned int    vao;
        unsigned int    vbo;
        unsigned int    ebo;

        ShaderProgram*  shader;
        Texture*        texture;
        mat4            model;
        vec3            pos;

        int             elementCount;

        void*           parent;

        void            (*setUniforms)(void*);
        void            (*unsetUniforms)(void*);
        void            (*render)(struct render_object_t* self);
        void            (*destroy)(struct render_object_t* self);
} RenderObject;

RenderObject* RenderObjectCreate(ShaderProgram* shader, 
                vec3 pos,
                int verticesSize, float* vertices, 
                int indicesSize, unsigned int* indices,
                void* parent,
                void (*setUniforms)(void*), void (*unsetUniforms)(void*));

vec3*         Position(float x, float y, float z);

#endif /* __RENDERER_H__ */
