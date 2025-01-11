#ifndef __TILE_MAP_H__
#define __TILE_MAP_H__

#include "renderer.h"
#include "shader.h"
#include "texture_atlas.h"

typedef struct tile_map_t
{
        RenderObject*           object;

        TextureAtlas*           textureAtlas;

        int                     tileRows;
        int                     tileCols;
        unsigned int*           tileMap;

        void                    ( *destroy )( struct tile_map_t* self );
} TileMap;

TileMap*                        TileMapCreate(ShaderProgram* program);

#endif /* __TILE_MAP_H__ */
