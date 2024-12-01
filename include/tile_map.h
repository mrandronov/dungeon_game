#ifndef __TILE_MAP_H__
#define __TILE_MAP_H__

#include "shader.h"
#include "texture_atlas.h"

typedef struct tile_map_t
{
        unsigned int            vbo;
        unsigned int            vao;
        unsigned int            ebo;
        ShaderProgram*          shader;
        TextureAtlas*           textureAtlas;

        void                    ( *render )( struct tile_map_t* self );
        void                    ( *destroy )( struct tile_map_t* self );
} TileMap;

TileMap*                        TileMapCreate( ShaderProgram* program, TextureAtlas* textureAtlas );

#endif /* __TILE_MAP_H__ */
