#include <stdlib.h>
#include <string.h>

#define GL_SILENCE_DEPRECATION
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <OpenGL/gl.h>
#include "cglm/mat4.h"

#include "logging.h"
#include "player.h"

void
PlayerSetUniforms(void* obj)
{
        Player* self = (Player*) obj;

        self->object->texture = self->currentTexture->getCurrentTexture(self->currentTexture);

        int flipTextureUniformLoc = glGetUniformLocation(self->object->shader->id, "flipTexture");
        glUniform1i(flipTextureUniformLoc, !self->isFacingRight);
}

void
PlayerUnsetUniforms(void* obj)
{
        Player* self = (Player*) obj;

        int flipTextureUniformLoc = glGetUniformLocation(self->object->shader->id, "flipTexture");
        glUniform1i(flipTextureUniformLoc, 0);
}

float*
initVertices(int verticesSize)
{
        float* vertices = (float*) malloc( verticesSize * sizeof(float) );

        float scale = 0.8f;

        float localVert[20] = {
                scale,  scale, 0.0f,        1.0f, 1.0f, // top right
                scale, -scale, 0.0f,        1.0f, 0.0f, // bottom right
                -scale, -scale, 0.0f,        0.0f, 0.0f, // bottom left
                -scale,  scale, 0.0f,        0.0f, 1.0f  // top left
        };
        memcpy(vertices, localVert, verticesSize * sizeof(float));

        return vertices;
}

unsigned int*
initIndices(int indicesSize)
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
PlayerSetPos(Player* self, float x, float y, float z) {
        
}

void
PlayerDestroy(Player* self)
{
        // TODO
}

Player*
PlayerCreate(ShaderProgram* shader)
{
        Player*         self = (Player*) malloc(sizeof(Player));

        int verticesSize = 20;
        int indicesSize = 6;

        self->object = RenderObjectCreate(shader,
                        GLM_MAT4_IDENTITY,
                        verticesSize, initVertices(verticesSize),
                        indicesSize, initIndices(indicesSize),
                        self,
                        PlayerSetUniforms, PlayerUnsetUniforms);

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

        self->destroy = PlayerDestroy;

        log_info("player: create success");

        return self;
}
