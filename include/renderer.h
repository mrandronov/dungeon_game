#ifndef __RENDERER_H__
#define __RENDERER_H__

#include "cglm/types.h"

#include "shader.h"
#include "texture.h"

typedef struct render_object_t
{
        unsigned int    vao;
        unsigned int    vbo;
        unsigned int    ebo;

        ShaderProgram*  shader;
        mat4            model;
        Texture*        texture;

        int             elementCount;

        void*           parent;

        void            (*setUniforms)(void*);
        void            (*unsetUniforms)(void*);
        void            (*render)(struct render_object_t* self);
        void            (*destroy)(struct render_object_t* self);
} RenderObject;


RenderObject* RenderObjectCreate(ShaderProgram* shader, 
                mat4 model, 
                int verticesSize, float* vertices, 
                int indicesSize, unsigned int* indices,
                void* parent,
                void (*setUniforms)(void*), void (*unsetUniforms)(void*));

#endif /* __RENDERER_H__ */
