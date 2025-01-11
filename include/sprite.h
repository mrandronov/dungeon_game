#ifndef __SPRITE_H__
#define __SPRITE_H__

#include "renderer.h"
#include "shader.h"
#include "texture.h"

typedef struct sprite_t
{
        RenderObject*      object;

        AnimatedTexture*   runTexture;
        AnimatedTexture*   idleTexture;
        AnimatedTexture*   currentTexture;

        bool            isFacingRight;

        void            (*destroy)(struct sprite_t* self);
} Sprite;

Sprite*                 SpriteCreate(ShaderProgram* shader);

#endif /* __SPRITE_H__ */
