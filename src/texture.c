#include <stdbool.h>

#define GL_SILENCE_DEPRECATION
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <OpenGL/gl.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "logging.h"
#include "texture.h"

char*                   sprite_path = "./res/Sprite.png";

Texture*
texture_create(char* image_path, bool flip)
{
        Texture*        self = (Texture*) malloc(sizeof(Texture));

        int width = 0;
        int height = 0;
        int num_color_channels = 0;

        self->image_path = image_path;
        
        glGenTextures(1, &self->id);
        glBindTexture(GL_TEXTURE_2D, self->id);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

        if (flip) {
                stbi_set_flip_vertically_on_load(true);
        }

        unsigned char* data = stbi_load(image_path, &width, &height, &num_color_channels, 0);
        if (data)
        {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
                glGenerateMipmap(GL_TEXTURE_2D);
                log_info("create texture for %s: success", image_path);
        }
        else
        {
                log_error("failed to load texture %s", image_path);
                free(self);
                self = NULL;
        }

        stbi_image_free(data);

        return self;
}

void
BindAnimatedTexture(AnimatedTexture* self)
{
        float           now = glfwGetTime();

        if ((now - self->lastFrameTime) >= (1.0f / self->rate))
        {
                self->currentFrame += 1;
                if ( self->currentFrame >= self->frameCount ) {
                        self->currentFrame = 0;
                }

                self->lastFrameTime = now;
        }
        glBindTexture(GL_TEXTURE_2D, self->frames[self->currentFrame].id);
}

Texture*
GetCurrentTexture(AnimatedTexture* self)
{
        float           now = glfwGetTime();

        if (self->isStarted && (now - self->lastFrameTime) >= (1.0f / self->rate))
        {
                self->currentFrame += 1;
                if ( self->currentFrame >= self->frameCount ) {
                        self->currentFrame = 0;
                }

                self->lastFrameTime = now;
        }

        return &self->frames[self->currentFrame];
}

void
AnimatedTextureStart(AnimatedTexture* self)
{
        self->isStarted = true;
}

void
AnimatedTextureStop(AnimatedTexture* self)
{
        self->isStarted = false;
}

AnimatedTexture*
AnimatedTextureCreate(float rate, int count, char** imagePaths, bool flip)
{
        AnimatedTexture* self = (AnimatedTexture*) malloc(sizeof(AnimatedTexture));

        self->rate = rate;
        self->lastFrameTime = 0.0f;
        self->currentFrame = 0;
        self->frameCount = count;
        self->frames = (Texture*) malloc( self->frameCount * sizeof(Texture));
        self->isStarted = false;

        self->getCurrentTexture = GetCurrentTexture;

        // Load each individual image
        for (int i = 0; i < count; i++) {
                self->frames[i] = *texture_create(imagePaths[i], flip);
        }

        return self;
}
