#define GL_SILENCE_DEPRECATION
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <OpenGL/gl.h>

#include "logging.h"
#include "texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture*
texture_create(char* image_path)
{
        Texture*        self = (Texture*)malloc(sizeof(Texture));

        self->image_path = image_path;

        glGenTextures(1, &self->id);
        glBindTexture(GL_TEXTURE_2D, self->id);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        unsigned char*  data = stbi_load(image_path, &self->width, &self->height, &self->num_color_channels, STBI_rgb);

        if (!data){
                log_error("failed to load texture %s", image_path);
                stbi_image_free(data);
                return self;
        }

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, self->width, self->height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(data);

        log_info("create texture for %s: success", image_path);

        return self;
}
