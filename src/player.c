#include "torch.h"
#include <stdlib.h>
#include <string.h>

#define GL_SILENCE_DEPRECATION
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <OpenGL/gl.h>
#include "cglm/cglm.h"

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
        float*          vertices = (float*) malloc( verticesSize * sizeof(float) );

        float           scale = 0.8f;

        float           localVert[20] =
        {
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
        unsigned int*   indices = (unsigned int*) malloc( indicesSize * sizeof(unsigned int) );

        unsigned int    localIndex[6] =
        {
                0, 1, 3, // first triangle
                1, 2, 3  // second triangle
        };

        memcpy(indices, localIndex, indicesSize * sizeof(unsigned int));

        return indices;
}

typedef struct point_t {
        float x;
        float y;
} Point;

typedef struct box_t {
        Point pTop;
        Point pBot;
} Box;

Box box1 = (Box) { 
        .pTop = { .x = 0.0f, .y = 0.0f },
        .pBot = { .x = 12.0f, .y = 1.0f },
};

bool
posInBox(vec3 pos, Box* box)
{
        float posX = pos[0];
        float posY = pos[1];

        if ( box->pTop.x <= posX && posX <= box->pBot.x && box->pTop.y <= posY && posY <= box->pBot.y )
        {
                return true;
        }


        return false;
}

bool
isColliding(vec3 temp)
{
        if (posInBox(temp, &box1))
        {
                return true;
        }

        return false; 
}

ShaderProgram* shader_program;

void
PlayerPlaceTorch(Player* self)
{
        TorchCreate(shader_program, self->object->pos);
}

void
PlayerMove(Player* self, vec3 vector)
{
        vec3 temp;
        glm_vec3_add(self->object->pos, vector, temp);

        if (!isColliding(temp)) {
                glm_vec3_copy(temp, self->object->pos);
        }
}

void
PlayerSetPos(Player* self, float x, float y, float z)
{
        
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

        int             verticesSize = 20;
        int             indicesSize = 6;

        self->object = RenderObjectCreate(shader,
                        (vec3) {1.0f, 2.0f, 0.0f},
                        verticesSize, initVertices(verticesSize),
                        indicesSize, initIndices(indicesSize),
                        self,
                        PlayerSetUniforms, PlayerUnsetUniforms);

        self->runTexture = AnimatedTextureCreate(GetPlayerRunTexConf());
        self->idleTexture = AnimatedTextureCreate(GetPlayerIdleTexConf());

        self->currentTexture = self->idleTexture;
        self->isFacingRight = true;

        self->placeTorch = PlayerPlaceTorch;
        self->move = PlayerMove;
        self->destroy = PlayerDestroy;

        return self;
}

TextureConfig
GetPlayerRunTexConf()
{
        int             textureCount = 6;
        char*           texturePaths[] =
        {
                "./res/player_sprites/run/adventurer-run-00.png",
                "./res/player_sprites/run/adventurer-run-01.png",
                "./res/player_sprites/run/adventurer-run-02.png",
                "./res/player_sprites/run/adventurer-run-03.png",
                "./res/player_sprites/run/adventurer-run-04.png",
                "./res/player_sprites/run/adventurer-run-05.png",
        };

        return (TextureConfig) {
            .rate = 15.0f,
            .numFrames = textureCount,
            .texturePaths = mallocPersistentPaths(textureCount, texturePaths),
            .flip = true,
            .loop = false
        };
}

TextureConfig
GetPlayerIdleTexConf()
{
        int             textureCount = 4;
        char*           texturePaths[] =
        {
                "./res/player_sprites/idle/adventurer-idle-00.png",
                "./res/player_sprites/idle/adventurer-idle-01.png",
                "./res/player_sprites/idle/adventurer-idle-02.png",
                "./res/player_sprites/idle/adventurer-idle-03.png",
        };

        return (TextureConfig) {
            .rate = 3.0f,
            .numFrames = textureCount,
            .texturePaths = mallocPersistentPaths(textureCount, texturePaths),
            .flip = true,
            .loop = false
        };
}

void
NilSetUniforms(void* obj)
{
}

void
NilUnsetUniforms(void* obj)
{
}

Circle*
CircleCreate(ShaderProgram* shader)
{
        Circle* self = malloc(sizeof(Circle));
        float* circle_vertices = malloc(sizeof(float) * 12);
        memcpy(
                circle_vertices,
                (float[12])  
                {
                        0.5f,  0.5f, 0.0f,  // top right
                        0.5f, -0.5f, 0.0f,  // bottom right
                       -0.5f, -0.5f, 0.0f,  // bottom left
                       -0.5f,  0.5f, 0.0f   // top left 
                },
                sizeof(float) * 12
        );

        unsigned int* circle_indices = malloc(sizeof(unsigned int) * 6);
        memcpy(
                circle_indices,
                (float[6])
                {
                        0, 1, 3,   // first triangle
                        1, 2, 3    // second triangle
                },
                sizeof(unsigned int) * 6
        );

        self->object = RenderObjectCreate(
                shader,
                (vec3) {0.0f, 0.0f, 0.0f},
                12, circle_vertices,
                6, circle_indices,
                self,
                NilSetUniforms, NilUnsetUniforms
        );
        self->object->texture = NULL;

        return self;
}
