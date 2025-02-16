#include "logging.h"
#include <stdlib.h>
#include <string.h>
#include <uuid/uuid.h>

#define GL_SILENCE_DEPRECATION
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <OpenGL/gl.h>

#include "renderer.h"
#include "camera.h"
#include "objects.h"


void
renderObjectRender(RenderObject* self)
{
        self->model[3][0] = self->pos[0];
        self->model[3][1] = self->pos[1];

        self->shader->setUniform(self->shader, "orthographic", MAT4, (float*) ortho);
        self->shader->setUniform(self->shader, "view", MAT4, (float*) view);
        self->shader->setUniform(self->shader, "model", MAT4, (float*) self->model);

        self->shader->activate(self->shader);

        self->setUniforms(self->parent);

        if (self->texture != NULL)
        {
                glBindTexture(GL_TEXTURE_2D, self->texture->id);
        }
        else
        {
                glBindTexture(GL_TEXTURE_2D, 0);
        }

        glBindVertexArray(self->vao);
        glDrawElements(GL_TRIANGLES, self->elementCount, GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);

        self->unsetUniforms(self->parent);
}

void
renderObjectDestroy(RenderObject* self)
{
        // TODO
}

char*
getUUID()
{
        uuid_t binUUID;

        uuid_generate_random(binUUID);

        char* uuid = malloc(37);

        uuid_unparse_lower(binUUID, uuid);

        return uuid;
}

RenderObject*
RenderObjectCreate(ShaderProgram* shader, 
                vec3 pos,
                int verticesSize, float* vertices, 
                int indicesSize, unsigned int* indices,
                void* parent,
                void (*setUniforms)(void*), void (*unsetUniforms)(void*))
{
        RenderObject* self = malloc(sizeof(RenderObject));

        self->id = getUUID();

        glGenBuffers(1, &self->vbo);
        glGenVertexArrays(1, &self->vao);
        glGenBuffers(1, &self->ebo);

        self->shader = shader;
        memcpy(self->model, GLM_MAT4_IDENTITY, sizeof(mat4));
        memcpy(self->pos, pos, sizeof(vec3));

        self->elementCount = indicesSize;

        self->parent = parent;

        self->setUniforms = setUniforms;
        self->unsetUniforms = unsetUniforms;
        self->render = renderObjectRender;
        self->destroy = renderObjectDestroy;

        glBindVertexArray(self->vao);

        glBindBuffer(GL_ARRAY_BUFFER, self->vbo);
        glBufferData(GL_ARRAY_BUFFER, verticesSize * sizeof(float), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, self->ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize * sizeof(int), indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) 0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) (3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        AddObject(self);

        return self;
}

vec3*
Position(float x, float y, float z)
{
        vec3* pos = malloc(sizeof(vec3));
        memcpy(pos, (vec3) {x, y, z}, sizeof(vec3));

        return pos;
}
