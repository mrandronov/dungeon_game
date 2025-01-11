#include <stdlib.h>

#include "texture_atlas.h"

TileTexture*
TileTextureCreate(enum TileType type, int offsetX, int offsetY)
{
        TileTexture*    self = (TileTexture*) malloc(sizeof(TileTexture));

        self->type = type;
        self->offsetX = offsetX;
        self->offsetY = offsetY;

        return self;
}

TileTexture*
getTexture(TextureAtlas* self, enum TileType type)
{
        switch (type) {
                case GRASS:
                        return self->grass;
                case DIRT:
                        return self->dirt;
                case STONE:
                        return self->stone;
                case WOOD:
                        return self->wood;
                default:
                        return self->grass;
        }
}

void
TextureLookup(TextureAtlas* self, enum TileType type, float* offsetX, float* offsetY)
{
        TileTexture*            tile = getTexture(self, type - 1);
        *offsetX = (float) tile->offsetX * tex_tile_unit;
        *offsetY = (float) tile->offsetY * tex_tile_unit;
}

TextureAtlas*
TextureAtlasCreate(char* texture_img_path)
{
        TextureAtlas*           self = (TextureAtlas*) malloc(sizeof(TextureAtlas));

        self->texture = texture_create(texture_img_path, false);

        self->grass = TileTextureCreate(GRASS, 2, 15);
        self->dirt = TileTextureCreate(DIRT, 2, 30);
        self->stone = TileTextureCreate(STONE, 7, 0);
        self->wood = TileTextureCreate(WOOD, 1, 13);

        self->lookup = TextureLookup;

        return self;
}
