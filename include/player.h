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
        
        void                    (*placeTorch)(struct player_t* self);
        void                    (*move)(struct player_t* self, vec3 vector);
        void                    (*destroy)(struct player_t* self);
} Player;

Player*                 PlayerCreate(ShaderProgram *shader);

TextureConfig           GetPlayerRunTexConf();
TextureConfig           GetPlayerIdleTexConf();

typedef struct circle_t
{
        RenderObject*           object;

        void                    (*move)(struct circle_t* self, vec3 vector);
        void                    (*destroy)(struct circle_t* self);
} Circle;

Circle*                 CircleCreate(ShaderProgram* shader);

#endif /* __PLAYER_H__ */
