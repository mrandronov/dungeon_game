#ifndef __TEXTURE_ATLAS_H__
#define __TEXTURE_ATLAS_H__

#include "texture.h"

#define tex_tile_unit 0.03125f

enum TileType
{
        GRASS,  // 1
        DIRT,   // 2
        STONE,  // 3
        WOOD,   // 4
};

typedef struct
{
        enum TileType           type;
        int                     offsetX;
        int                     offsetY;
} TileTexture;

typedef struct texture_atlas_t
{
        Texture*                texture;
        TileTexture*            grass;
        TileTexture*            dirt;
        TileTexture*            stone;
        TileTexture*            wood;
        
        void                    ( *lookup )( struct texture_atlas_t* self, enum TileType type, float* offsetX, float* offsetY );  
} TextureAtlas;

TextureAtlas*                   TextureAtlasCreate( char* texture_img_path );

#endif /* __TEXTURE_ATLAS_H__ */
