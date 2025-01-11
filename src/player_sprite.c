#include <stdlib.h>
#include <string.h>

#define GL_SILENCE_DEPRECATION
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <OpenGL/gl.h>

#include "logging.h"
#include "player_sprite.h"
#include "camera.h"
#include "texture.h"

void
PlayerSpriteRender(PlayerSprite* self)
{
        self->shader->setUniform(self->shader, "orthographic", MAT4, (float*) ortho);
        self->shader->setUniform(self->shader, "view", MAT4, (float*) view);
        self->shader->setUniform(self->shader, "model", MAT4, (float*) self->model);

        self->shader->activate(self->shader);

        int flipTextureUniformLoc = glGetUniformLocation(self->shader->id, "flipTexture");
        glUniform1i(flipTextureUniformLoc, !self->isFacingRight);

        BindAnimatedTexture(self->currentTexture);

        glBindVertexArray(self->vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glUniform1i(flipTextureUniformLoc, 0);
}

void
PlayerSpriteInitModel(PlayerSprite* self)
{
        self->vertices = (float*) malloc( self->verticesSize * sizeof(float) );
        self->indices = (unsigned int*) malloc( self->indicesSize * sizeof(unsigned int) );

        float localVert[20] = {
                0.5f,  0.5f, 0.0f,        1.0f, 1.0f, // top right
                0.5f, -0.5f, 0.0f,        1.0f, 0.0f, // bottom right
                -0.5f, -0.5f, 0.0f,        0.0f, 0.0f, // bottom left
                -0.5f,  0.5f, 0.0f,        0.0f, 1.0f  // top left
        };
        memcpy(self->vertices, localVert, self->verticesSize * sizeof(float));

        unsigned int localIndex[6] = {
                0, 1, 3, // first triangle
                1, 2, 3  // second triangle
        };
        memcpy(self->indices, localIndex, self->indicesSize * sizeof(unsigned int));
}

void
PlayerSpriteDestroy(PlayerSprite* self)
{
        free(self->vertices);
        free(self->indices);
}

PlayerSprite*
PlayerSpriteCreate(ShaderProgram* shader)
{
        PlayerSprite*         self = (PlayerSprite*) malloc(sizeof(PlayerSprite));

        glGenBuffers(1, &self->vbo);
        glGenVertexArrays(1, &self->vao);
        glGenBuffers(1, &self->ebo);

        self->verticesSize = 20;
        self->indicesSize = 6;

        PlayerSpriteInitModel(self);

        self->shader = shader;

        char* runPaths[6] = {
                "./res/player_sprites/run/adventurer-run-00.png",
                "./res/player_sprites/run/adventurer-run-01.png",
                "./res/player_sprites/run/adventurer-run-02.png",
                "./res/player_sprites/run/adventurer-run-03.png",
                "./res/player_sprites/run/adventurer-run-04.png",
                "./res/player_sprites/run/adventurer-run-05.png",
        };

        char* idlePaths[4] = {
                "./res/player_sprites/idle/adventurer-idle-00.png",
                "./res/player_sprites/idle/adventurer-idle-01.png",
                "./res/player_sprites/idle/adventurer-idle-02.png",
                "./res/player_sprites/idle/adventurer-idle-03.png",
        };

        self->runTexture = AnimatedTextureCreate(15.0f, 6, runPaths, true);
        self->idleTexture = AnimatedTextureCreate(3.0f, 4, idlePaths, true);
        self->currentTexture = self->idleTexture;

        self->isFacingRight = true;

        self->render = PlayerSpriteRender;
        self->destroy = PlayerSpriteDestroy;

        glBindVertexArray(self->vao);

        glBindBuffer(GL_ARRAY_BUFFER, self->vbo);
        glBufferData(GL_ARRAY_BUFFER, self->verticesSize * sizeof(float), self->vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, self->ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, self->indicesSize * sizeof(int), self->indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) 0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) (3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        log_info("create sprite: success");

        return self;
}

