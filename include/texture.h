#ifndef __TEXTURE_H__
#define __TEXTURE_H__

typedef struct
{
        unsigned int    id;
        int             width;
        int             height;
        int             num_color_channels;
        char*           image_path;
} Texture;

Texture*                texture_create( char* image_path );

#endif /* __TEXTURE_H__ */
