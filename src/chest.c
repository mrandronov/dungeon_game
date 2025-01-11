#include <stdlib.h>
#include <string.h>

#define GL_SILENCE_DEPRECATION
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <OpenGL/gl.h>
#include "cglm/cglm.h"

#include "logging.h"
#include "chest.h"

char*           idle_chest_texture_path = "./res/Animated Chests/Chests/chest-normal-01.png";

void
ChestSetUniforms(void* obj)
{
        Chest* self = (Chest*) obj;

        self->object->texture = self->openingTexture->getCurrentTexture(self->openingTexture);
}

void
ChestUnsetUniforms(void* obj)
{
        /*Chest* self = (Chest*) obj;*/
}

float*
chestInitVertices(int verticesSize)
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
chestInitIndices(int indicesSize)
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
ChestDestroy(Chest* self)
{
        // TODO
}

Chest*
ChestCreate(ShaderProgram* shader)
{
        Chest* self = malloc(sizeof(Chest));

        int verticesSize = 20;
        int indicesSize = 6;

        mat4 model_pos;
        memcpy(&model_pos, &GLM_MAT4_IDENTITY, sizeof(mat4));
        glm_translate(model_pos, (vec3) {1.0f, 2.0f, 0.0f});

        self->object = RenderObjectCreate(shader,
                        model_pos,
                        verticesSize, chestInitVertices(verticesSize),
                        indicesSize, chestInitIndices(indicesSize),
                        self,
                        ChestSetUniforms, ChestUnsetUniforms);

        self->idleTexture = texture_create(idle_chest_texture_path, false);
        self->object->texture = self->idleTexture;

        char* open_texture_paths[10] = {
                "./res/Animated Chests/Chests/chest-normal-01.png",
                "./res/Animated Chests/Chests/chest-normal-02.png",
                "./res/Animated Chests/Chests/chest-normal-03.png",
                "./res/Animated Chests/Chests/chest-normal-04.png",
                "./res/Animated Chests/Chests/chest-normal-05.png",
                "./res/Animated Chests/Chests/chest-normal-06.png",
                "./res/Animated Chests/Chests/chest-normal-07.png",
                "./res/Animated Chests/Chests/chest-normal-08.png",
                "./res/Animated Chests/Chests/chest-normal-09.png",
                "./res/Animated Chests/Chests/chest-normal-10.png"
        };

        self->openingTexture = AnimatedTextureCreate(10.0f, 10, open_texture_paths, false);

        self->destroy = ChestDestroy;

        log_info("chest create: success");

        return self;
}
