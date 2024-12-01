#ifndef __SPRITE_H__
#define __SPRITE_H__

#include "cglm/mat4.h"

#include "shader.h"

typedef struct sprite_t
{
        unsigned int    vbo;
        unsigned int    vao;
        unsigned int    ebo;
        ShaderProgram*  shader;
        mat4            model;

        void            ( *render )( struct sprite_t* self );
        void            ( *destroy )( struct sprite_t* self );
} Sprite;

Sprite* SpriteCreate( ShaderProgram* shader, vec3 pos );

#endif /* __SPRITE_H__ */
