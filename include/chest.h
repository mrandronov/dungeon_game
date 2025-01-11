#ifndef __CHEST_H__
#define __CHEST_H__

#include "renderer.h"
typedef struct chest_t
{
        RenderObject*           object;

        Texture*                idleTexture;
        AnimatedTexture*        openingTexture;

        void                    (*destroy)(struct chest_t* self);
} Chest;

Chest*                          ChestCreate(ShaderProgram* shader);

#endif /* __CHEST_H__ */
