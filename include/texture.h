#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include <stdbool.h>

typedef struct
{
        int             rate;
        int             numFrames;
        char**          texturePaths;
        bool            flip;
        bool            loop;
} TextureConfig;

typedef struct
{
        unsigned int    id;
        char*           image_path;
} Texture;

Texture*                texture_create(char* image_path, bool flip);

typedef struct animated_texture_t
{
        float           rate;
        float           lastFrameTime;
        int             currentFrame;
        int             frameCount;
        Texture*        frames;

        bool            isStarted;
        bool            loop;

        Texture*        (*getCurrentTexture)(struct animated_texture_t* self);
} AnimatedTexture;

void                    BindAnimatedTexture(AnimatedTexture* self);
AnimatedTexture*        AnimatedTextureCreate(TextureConfig config);

char**                  mallocPersistentPaths(int textureCount, char** texturePaths);
TextureConfig           GetTorchTextConf();

#endif /* __TEXTURE_H__ */
