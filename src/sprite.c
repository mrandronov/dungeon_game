#include <stdlib.h>
#include <string.h>

#define GL_SILENCE_DEPRECATION
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <OpenGL/gl.h>
#include "cglm/mat4.h"

#include "logging.h"
#include "sprite.h"
#include "texture.h"

void
SpriteSetUniforms(void* obj)
{
        Sprite* self = (Sprite*) obj;

        self->object->texture = self->currentTexture->getCurrentTexture(self->currentTexture);

        int flipTextureUniformLoc = glGetUniformLocation(self->object->shader->id, "flipTexture");
        glUniform1i(flipTextureUniformLoc, !self->isFacingRight);
}

void
SpriteUnsetUniforms(void* obj)
{
        Sprite* self = (Sprite*) obj;

        int flipTextureUniformLoc = glGetUniformLocation(self->object->shader->id, "flipTexture");
        glUniform1i(flipTextureUniformLoc, 0);
}

float*
spriteInitVertices(int verticesSize)
{
        float* vertices = (float*) malloc( verticesSize * sizeof(float) );

        float localVert[20] = {
                0.5f,  0.5f, 0.0f,        1.0f, 1.0f, // top right
                0.5f, -0.5f, 0.0f,        1.0f, 0.0f, // bottom right
                -0.5f, -0.5f, 0.0f,        0.0f, 0.0f, // bottom left
                -0.5f,  0.5f, 0.0f,        0.0f, 1.0f  // top left
        };
        memcpy(vertices, localVert, verticesSize * sizeof(float));

        return vertices;
}

unsigned int*
spriteInitIndices(int indicesSize)
{
        unsigned int* indices = (unsigned int*) malloc( indicesSize * sizeof(unsigned int) );

        unsigned int localIndex[6] = {
                0, 1, 3, // first triangle
                1, 2, 3  // second triangle
        };
        memcpy(indices, localIndex, indicesSize * sizeof(unsigned int));

        return indices;
}

void
SpriteDestroy(Sprite* self)
{
        // TODO
}

Sprite*
SpriteCreate(ShaderProgram* shader)
{
        Sprite*         self = (Sprite*) malloc(sizeof(Sprite));

        int verticesSize = 20;
        int indicesSize = 6;

        self->object = RenderObjectCreate(shader,
                        GLM_MAT4_IDENTITY,
                        verticesSize, spriteInitVertices(verticesSize),
                        indicesSize, spriteInitIndices(indicesSize),
                        self,
                        SpriteSetUniforms, SpriteUnsetUniforms);

        char* runTexturePaths[6] = {
                "./res/player_sprites/run/adventurer-run-00.png",
                "./res/player_sprites/run/adventurer-run-01.png",
                "./res/player_sprites/run/adventurer-run-02.png",
                "./res/player_sprites/run/adventurer-run-03.png",
                "./res/player_sprites/run/adventurer-run-04.png",
                "./res/player_sprites/run/adventurer-run-05.png",
        };

        char* idleTexturePaths[4] = {
                "./res/player_sprites/idle/adventurer-idle-00.png",
                "./res/player_sprites/idle/adventurer-idle-01.png",
                "./res/player_sprites/idle/adventurer-idle-02.png",
                "./res/player_sprites/idle/adventurer-idle-03.png",
        };

        self->runTexture = AnimatedTextureCreate(15.0f, 6, runTexturePaths, true);
        self->idleTexture = AnimatedTextureCreate(3.0f, 4, idleTexturePaths, true);
        self->currentTexture = self->idleTexture;

        self->isFacingRight = true;

        self->destroy = SpriteDestroy;

        log_info("sprite: create success");

        return self;
}

