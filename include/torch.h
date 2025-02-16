#ifndef __TORCH_H__
#define __TORCH_H__

#include "renderer.h"

typedef struct
{
        ShaderProgram*  shader;
        vec3*           pos;
        TextureConfig   textureConfig;
} SpriteConfig;

typedef struct static_sprite_t
{
        RenderObject*           object;

        Texture*                idleTexture;
        AnimatedTexture*        currentTexture;

        void                    (*destroy)(struct static_sprite_t* self);
} StaticSprite;

StaticSprite*                   StaticSpriteCreate(SpriteConfig config);

StaticSprite*                   TorchCreate(ShaderProgram* shader, vec3 pos);

#endif /* __TORCH_H__ */
