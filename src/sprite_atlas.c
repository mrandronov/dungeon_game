#include <stdlib.h>

#include "sprite_atlas.h"

SpriteAtlas*
SpriteAtlasCreate( char* sprite_image_path )
{
        SpriteAtlas*            self = ( SpriteAtlas* ) malloc( sizeof( SpriteAtlas ) );

        self->texture = texture_create( sprite_image_path );

        return self;
}
