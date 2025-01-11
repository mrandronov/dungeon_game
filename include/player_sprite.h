#ifndef __PLAYER_SPRITE_H__
#define __PLAYER_SPRITE_H__

#include "cglm/types.h"

#include "shader.h"
#include "texture.h"

typedef struct player_sprite_t
{
        unsigned int    vbo;
        unsigned int    vao;
        unsigned int    ebo;
        
        ShaderProgram*  shader;

        AnimatedTexture*   runTexture;
        AnimatedTexture*   idleTexture;
        AnimatedTexture*   currentTexture;

        mat4            model;

        int             verticesSize;
        float*          vertices;
        int             indicesSize;
        unsigned int*   indices;

        bool            isFacingRight;

        void            (*render)(struct player_sprite_t* self);
        void            (*destroy)(struct player_sprite_t* self);
} PlayerSprite;

PlayerSprite*           PlayerSpriteCreate(ShaderProgram* shader);

#endif /* __PLAYER_SPRITE_H__ */
