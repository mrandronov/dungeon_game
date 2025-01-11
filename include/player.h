#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "renderer.h"
#include "shader.h"

typedef struct player_t
{
        RenderObject*           object;

        AnimatedTexture*        runTexture;
        AnimatedTexture*        idleTexture;
        AnimatedTexture*        currentTexture;

        bool                    isFacingRight;

        void                    (*destroy)(struct player_t* self);
} Player;

Player* PlayerCreate(ShaderProgram *shader);

#endif /* __PLAYER_H__ */
