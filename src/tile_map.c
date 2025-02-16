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
#include "renderer.h"

char*                   tile_map_path = "./maps/tilemap1.map";
char*                   texture_atlas_path = "./res/Textures.png";

void TileMapSetUniforms(void* obj)
{
}

void TileMapUnsetUniforms(void* obj)
{
}

void
TileMapDestroy(TileMap* self)
{
        // TODO
        // Need to destroy texture data as well
}

void
ReadMapData(TileMap* self, const char* map_file_path)
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

        char            c = ' ';
        int             newLineCount = 0;
        int             lineValueCount = 0;
        bool            lineValueFound = false;
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

        self->tileRows = newLineCount;
        self->tileCols = lineValueCount + 1;
        
        log_debug("found map of size %d x %d", self->tileRows, self->tileCols);

        c = ' ';
        rewind(fptr);

        // Now read values
        self->tileMap = (unsigned int*) malloc(self->tileRows * self->tileCols * sizeof(unsigned int)); 
        for (int i = 0; i < self->tileRows; i++)
        {
                for(int j = 0; j < self->tileCols; j++)
                {
                        fscanf(fptr, "%d", &self->tileMap[ (i * self->tileRows) + j ]);
                }
        }

        log_info("load map: success");
}

float*
GenerateVertices(TileMap* self)
{
        int rows = self->tileRows;
        int cols = self->tileCols;

        int verticesSize = rows * cols * (4 * 5);
        float*          vertices = (float*) malloc(verticesSize * sizeof(float));

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
                        int             tileType = self->tileMap[ (i * rows) + j ];

                        self->textureAtlas->lookup(self->textureAtlas, tileType, &texOffsetX, &texOffsetY); 

                        // top right
                        vertices[ vertIndex ] = offsetX + 0.5f;                 // x
                        vertices[ vertIndex + 2 ] = offsetY + 0.5f;             // y
                        vertices[ vertIndex + 1 ] = 0.0f;                       // z
                        vertices[ vertIndex + 3 ] = texOffsetX + 0.03125f;      // texture x
                        vertices[ vertIndex + 4 ] = texOffsetY + 0.0f;          // texture y
                        
                        // bottom right
                        vertIndex += 5;
                        vertices[ vertIndex ] = offsetX + 0.5f;                 // x
                        vertices[ vertIndex + 2 ] = offsetY - 0.5f;             // y
                        vertices[ vertIndex + 1 ] = 0.0f;                       // z
                        vertices[ vertIndex + 3 ] = texOffsetX + 0.03125f;      // texture x
                        vertices[ vertIndex + 4 ] = texOffsetY + 0.03125f;      // texture y

                        // bottom left
                        vertIndex += 5;
                        vertices[ vertIndex ] = offsetX - 0.5f;                 // x
                        vertices[ vertIndex + 2 ] = offsetY - 0.5f;             // y
                        vertices[ vertIndex + 1 ] = 0.0f;                       // z
                        vertices[ vertIndex + 3 ] = texOffsetX + 0.0f;          // texture x
                        vertices[ vertIndex + 4 ] = texOffsetY + 0.03125f;      // texture y

                        // top left
                        vertIndex += 5;
                        vertices[ vertIndex ] = offsetX - 0.5f;                 // x
                        vertices[ vertIndex + 2 ] = offsetY + 0.5f;             // y
                        vertices[ vertIndex + 1 ] = 0.0f;                       // z
                        vertices[ vertIndex + 3 ] = texOffsetX + 0.0f;          // texture x
                        vertices[ vertIndex + 4 ] = texOffsetY + 0.0f;          // texture y
                }
        }
        
        return vertices;
}

unsigned int*
GenerateIndices(TileMap* self)
{
        int             rows            = self->tileRows;
        int             cols            = self->tileCols;
        int             indicesSize     = rows * cols * 6; 
        unsigned int*   indices         = (unsigned int*) malloc(indicesSize * sizeof(unsigned int));

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
TileMapCreate(ShaderProgram* shader)
{
        TileMap*               self = (TileMap*) malloc(sizeof(TileMap));

        self->textureAtlas = TextureAtlasCreate(texture_atlas_path);

        ReadMapData(self, tile_map_path);

        int             verticesSize    = self->tileRows * self->tileCols * (4 * 5);
        float*          vertices        = GenerateVertices(self);

        int             indicesSize     = self->tileRows * self->tileCols * 6; 
        unsigned int*   indices         = GenerateIndices(self);

        self->object = RenderObjectCreate(
                        shader,
                        (vec3) {0.0f, 0.0f, 0.0f},
                        verticesSize, vertices,
                        indicesSize, indices,
                        self,
                        TileMapSetUniforms, TileMapUnsetUniforms);
        self->object->texture = self->textureAtlas->texture;

        self->destroy = TileMapDestroy;

        log_info("create tile map: success");

        return self;
}

