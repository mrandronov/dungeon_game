#include <stdlib.h>
#include <string.h>

#include "bounds.h"
#include "logging.h"

void
BoundsSetUniforms(void* obj)
{

}

void
BoundsUnsetUniforms(void* obj)
{

}

float*
boundsInitVertices(int verticesSize)
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
boundsInitIndices(int indicesSize)
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

Bounds*
BoundsCreate(ShaderProgram* shader, vec4 color, vec3 pos)
{
        Bounds*         self = malloc(sizeof(Bounds));

        memcpy(self->color, color, sizeof(vec3));
        memcpy(self->pos, pos, sizeof(vec4));

        int             verticesSize = 20;
        int             indicesSize = 6;

        self->object = RenderObjectCreate(shader,
                        pos,
                        verticesSize, boundsInitVertices(verticesSize),
                        indicesSize, boundsInitIndices(indicesSize),
                        self,
                        BoundsSetUniforms, BoundsUnsetUniforms);

        log_info("bounds create: success");

        return self;
}
