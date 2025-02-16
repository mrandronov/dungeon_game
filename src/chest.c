#include <stdlib.h>
#include <string.h>

#define GL_SILENCE_DEPRECATION
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <OpenGL/gl.h>
#include "cglm/cglm.h"

#include "logging.h"
#include "chest.h"
#include "camera.h"

char*           idle_chest_texture_path = "./res/Animated Chests/Chests/chest-normal-01.png";

typedef struct {
        float X;
        float Y;
} Point;

float
getScreenSpace(float P, float Pnd, float dim)
{
        return (Pnd + 1) * (dim / 2) + P;
}

Point
getScreenPoint(mat4* model, float H, float W, float X, float Y)
{
        mat4 result;
        glm_mat4_mulN((mat4 *[]){model, &view, &ortho}, 3, result);
        /*glm_mat4_print(result, stdout);*/
        /*log_debug("chest: hmmm %f", result[2][1] / result[2][3]);*/

        float Xnd = -(result[2][0]/4.5);
        float Ynd = (result[2][1]/4.5);

        float Xw = getScreenSpace(X, Xnd, W);
        float Yw = getScreenSpace(Y, Ynd, H);

        return (Point){ .X =  Xw, .Y = Yw };
}

bool
isMouseOverChest(Chest* self)
{
        /*Point p1 = getScreenPoint(&self->object->model, 1100, 900, -0.5f, -0.5f);*/
        /*log_debug("Xw = %f Yw = %f, Mx = %f My = %f", p1.X, p1.Y, mouseX, mouseY);*/

        // This is just about accurate
        // Now we need to figure out how to determine the area of the chest

        return true;
}

void
ChestSetUniforms(void* obj)
{
        Chest* self = (Chest*) obj;

        self->object->texture = self->openingTexture->getCurrentTexture(self->openingTexture);

        isMouseOverChest(self);
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
ChestCreate(ShaderProgram* shader, vec3 pos)
{
        Chest* self = malloc(sizeof(Chest));

        int verticesSize = 20;
        int indicesSize = 6;

        self->object = RenderObjectCreate(shader,
                        pos,
                        verticesSize, chestInitVertices(verticesSize),
                        indicesSize, chestInitIndices(indicesSize),
                        self,
                        ChestSetUniforms, ChestUnsetUniforms);

        self->idleTexture = texture_create(idle_chest_texture_path, false);
        self->object->texture = self->idleTexture;

        self->openingTexture = AnimatedTextureCreate(GetChestOpenTexConf());

        self->destroy = ChestDestroy;

        log_info("chest create: success");

        return self;
}

TextureConfig
GetChestOpenTexConf() 
{
        int textureCount = 10;
        char* texturePaths[] = {
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

        return (TextureConfig){
                .rate = 10.0f,
                .numFrames = textureCount,
                .texturePaths = mallocPersistentPaths(textureCount, texturePaths),
        };
}
