#ifndef __SPRITE_ATLAS_H__
#define __SPRITE_ATLAS_H__

#include "texture.h"

typedef struct sprite_atlas_t
{
        Texture*        texture;

} SpriteAtlas;

SpriteAtlas*            SpriteAtlasCreate( char* sprite_img_path );

#endif /* __SPRITE_ATLAS_H__ */
