#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#define GL_SILENCE_DEPRECATION
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <OpenGL/gl.h>
#include "cglm/mat4.h"

#include "logging.h"
#include "tile_map.h"
#include "texture_atlas.h"
#include "camera.h"

#define MAP_PATH "./maps/tilemap1.map"

int                     verticesSize;
float*                  vertices;

int                     indicesSize;
unsigned int*           indices;

int                     tileRows;
int                     tileCols;
unsigned int*           tileMap;

mat4                    model = GLM_MAT4_IDENTITY;

void
TileMapRender(TileMap* self)
{
        self->shader->setUniform(self->shader, "model", MAT4, (float*) model );
        self->shader->setUniform(self->shader, "view", MAT4, (float*) view );
        self->shader->setUniform(self->shader, "orthographic", MAT4, (float*) ortho );

        self->shader->activate(self->shader);

        glBindTexture(GL_TEXTURE_2D, self->textureAtlas->texture->id);
        glBindVertexArray(self->vao);

        glDrawElements(GL_TRIANGLES, 6 * tileRows * tileCols, GL_UNSIGNED_INT, 0);
        
        glBindVertexArray(0);
}

void
ReadMapData(const char* map_file_path)
{
        FILE*           fptr = NULL;

        fptr = fopen(map_file_path, "r");
        if (!fptr)
        {
                log_error("failed to open map file %s: %s", map_file_path, strerror(errno));
                exit(1);
        }

        log_info("loading tile map from [ %s ]", map_file_path);

        // Count new lines to get number of rows
        // Count space separated values to get number of columns

        int             newLineCount = 0;
        int             lineValueCount = 0;
        bool            lineValueFound = false;
        char            c = ' ';
        while (c != EOF)
        {
                c = fgetc(fptr);
                if (!lineValueFound && c == ' ')
                {
                        lineValueCount++;
                }
                else if (c == '\n')
                {
                        newLineCount++;
                        lineValueFound = true;
                }
        }

        tileRows = newLineCount;
        tileCols = lineValueCount + 1;
        
        log_info("found map of size %d x %d", tileRows, tileCols);

        c = ' ';
        rewind(fptr);

        // Now read values
        tileMap = (unsigned int*) malloc(tileRows * tileCols * sizeof(unsigned int ) ); 
        for (int i = 0; i < tileRows; i++)
        {
                for(int j = 0; j < tileCols; j++)
                {
                        fscanf(fptr, "%d", &tileMap[ (i * tileRows) + j ] );
                }
        }

        log_info("load map: success");
}

float*
GenerateVertices(TileMap* self, unsigned int* tileMap, int rows, int cols)
{
        verticesSize = rows * cols * (4 * 5);
        
        float*          vertices = (float*) malloc(verticesSize * sizeof(float ) );

        float           startX = 0.0f;
        float           startY = 0.0f;
        for (int i = 0; i < rows; i++)
        {
                float           offsetX = i + startX;
                for (int j = 0; j < cols; j++)
                {
                        float           texOffsetX = 0.0f;
                        float           texOffsetY = 0.0f;
                        float           offsetY = j + startY;
                        int             vertIndex = ((i * rows) + j) * 5 * 4;
                        int             tileType = tileMap[ (i * rows) + j ];

                        self->textureAtlas->lookup(self->textureAtlas, tileType, &texOffsetX, &texOffsetY); 

                        // top right
                        vertices[ vertIndex ] = offsetX + 0.5f;                 // x
                        vertices[ vertIndex + 1 ] = offsetY + 0.5f;             // y
                        vertices[ vertIndex + 2 ] = 0.0f;                       // z
                        vertices[ vertIndex + 3 ] = texOffsetX + 0.03125f;      // texture x
                        vertices[ vertIndex + 4 ] = texOffsetY + 0.0f;          // texture y
                        
                        // bottom right
                        vertIndex += 5;
                        vertices[ vertIndex ] = offsetX + 0.5f;                 // x
                        vertices[ vertIndex + 1 ] = offsetY - 0.5f;             // y
                        vertices[ vertIndex + 2 ] = 0.0f;                       // z
                        vertices[ vertIndex + 3 ] = texOffsetX + 0.03125f;      // texture x
                        vertices[ vertIndex + 4 ] = texOffsetY + 0.03125f;      // texture y

                        // bottom left
                        vertIndex += 5;
                        vertices[ vertIndex ] = offsetX - 0.5f;                 // x
                        vertices[ vertIndex + 1 ] = offsetY - 0.5f;             // y
                        vertices[ vertIndex + 2 ] = 0.0f;                       // z
                        vertices[ vertIndex + 3 ] = texOffsetX + 0.0f;          // texture x
                        vertices[ vertIndex + 4 ] = texOffsetY + 0.03125f;      // texture y

                        // top left
                        vertIndex += 5;
                        vertices[ vertIndex ] = offsetX - 0.5f;                 // x
                        vertices[ vertIndex + 1 ] = offsetY + 0.5f;             // y
                        vertices[ vertIndex + 2 ] = 0.0f;                       // z
                        vertices[ vertIndex + 3 ] = texOffsetX + 0.0f;          // texture x
                        vertices[ vertIndex + 4 ] = texOffsetY + 0.0f;          // texture y
                }
        }
        
        return vertices;
}

unsigned int*
GenerateIndices(unsigned int* tileMap, int rows, int cols)
{
        indicesSize = rows * cols * 6; 
        unsigned int*           indices = (unsigned int*) malloc(indicesSize * sizeof(unsigned int ) );

        int                     offset = 0;
        for (int i = 0; i < indicesSize; i += 6)
        {
                indices[ i ] = offset;
                indices[ i + 1 ] = offset + 1;
                indices[ i + 2 ] = offset + 3;

                indices[ i + 3 ] = offset + 1;
                indices[ i + 4 ] = offset + 2;
                indices[ i + 5 ] = offset + 3;
                offset += 4;
        }

        return indices;
}

TileMap*
TileMapCreate(ShaderProgram* shader, TextureAtlas* textureAtlas)
{
        TileMap*               self = (TileMap*) malloc(sizeof(TileMap ) );

        glGenBuffers(1, &self->vbo);
        glGenVertexArrays(1, &self->vao);
        glGenBuffers(1, &self->ebo);

        self->shader = shader;
        self->textureAtlas = textureAtlas;

        self->render = TileMapRender;

        ReadMapData(MAP_PATH);
        vertices = GenerateVertices(self, tileMap, tileRows, tileCols);
        indices = GenerateIndices(tileMap, tileRows, tileCols);

        glBindVertexArray(self->vao);

        glBindBuffer(GL_ARRAY_BUFFER, self->vbo);
        glBufferData(GL_ARRAY_BUFFER, verticesSize * sizeof(float), vertices, GL_STATIC_DRAW );

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, self->ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize * sizeof(unsigned int), indices, GL_STATIC_DRAW );

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void* ) 0 );
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void* ) (3 * sizeof(float ) ) );
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        memcpy(&view, &GLM_MAT4_IDENTITY, sizeof(GLM_MAT4_IDENTITY) );
        glm_translate(view, (vec3){ 0.0f, 0.0f, -10.0f } );
        glm_perspective_default(1.0f, ortho);

        log_info("create tile map: success");

        return self;
}

