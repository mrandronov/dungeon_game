#include "logging.h"
#include <stdlib.h>
#include <string.h>

#define GL_SILENCE_DEPRECATION
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <OpenGL/gl.h>
#include "cglm/cglm.h"

#include "torch.h"

void
StaticSpriteSetUniforms(void* obj)
{
        StaticSprite* self = (StaticSprite*) obj;

        self->object->texture = self->currentTexture->getCurrentTexture(self->currentTexture);
}

void
StaticSpriteUnsetUniforms(void* obj)
{
        /*StaticSprite* self = (StaticSprite*) obj;*/
}

float*
staticSpriteInitVertices(int verticesSize)
{
        float* vertices = (float*) malloc( verticesSize * sizeof(float) );

        float scale = 0.5f;
        float xunit = 1.0f;
        float yunit = 1.0f;

        float localVert[20] = {
                scale,  scale, 0.0f,        xunit, yunit, // top right
                scale, -scale, 0.0f,        xunit, 0.0f, // bottom right
                -scale, -scale, 0.0f,        0.0f, 0.0f, // bottom left
                -scale,  scale, 0.0f,        0.0f, yunit  // top left
        };
        memcpy(vertices, localVert, verticesSize * sizeof(float));

        return vertices;
}

unsigned int*
staticSpriteInitIndices(int indicesSize)
{
        unsigned int* indices = (unsigned int*) malloc(indicesSize * sizeof(unsigned int));

        unsigned int localIndex[6] = {
                0, 1, 3, // first triangle
                1, 2, 3  // second triangle
        };
        memcpy(indices, localIndex, indicesSize * sizeof(unsigned int));

        return indices;
}

void
StaticSpriteDestroy(StaticSprite* self)
{
        // TODO
}

StaticSprite*
StaticSpriteCreate(SpriteConfig config)
{
        StaticSprite* self = malloc(sizeof(StaticSprite));

        int verticesSize = 20;
        int indicesSize = 6;

        self->object = RenderObjectCreate(
                        config.shader,
                        *config.pos,
                        verticesSize, staticSpriteInitVertices(verticesSize),
                        indicesSize, staticSpriteInitIndices(indicesSize),
                        self,
                        StaticSpriteSetUniforms, StaticSpriteUnsetUniforms
                );

        self->object->texture = self->idleTexture;

        self->currentTexture = AnimatedTextureCreate(config.textureConfig);

        self->destroy = StaticSpriteDestroy;

        log_info("static sprite create: success");

        return self;
}

StaticSprite*
TorchCreate(ShaderProgram* shader, vec3 pos)
{
        vec3 passPos;

        memcpy(passPos, pos, sizeof(vec3));

        return StaticSpriteCreate(
                        (SpriteConfig) {
                                .shader = shader,
                                .pos = &passPos,
                                .textureConfig = GetTorchTextConf()
                        });
}

