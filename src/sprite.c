#include <stdlib.h>
#include <string.h>

#define GL_SILENCE_DEPRECATION
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <OpenGL/gl.h>

#include "sprite.h"
#include "camera.h"

int                     verticesSize;
float*                  vertices;

int                     indicesSize;
unsigned int*           indices;

void
SpriteRender( Sprite* self )
{
        self->shader->setUniform( self->shader, "model", MAT4, ( float* ) self->model );
        self->shader->setUniform( self->shader, "view", MAT4, ( float* ) view );
        self->shader->setUniform( self->shader, "orthographic", MAT4, ( float* ) ortho );

        self->shader->activate( self->shader );

        // TODO: Figure out the texture situation
        /*glBindTexture( GL_TEXTURE_2D, self->textureAtlas->texture->id );*/
        glBindVertexArray( self->vao );

        glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0 );
        
        glBindVertexArray( 0 );
}

void
SpriteDestroy( Sprite* self )
{
        // TODO
}

Sprite*
SpriteCreate( ShaderProgram* shader, vec3 pos )
{
        Sprite*         self = ( Sprite* ) malloc( sizeof( Sprite ) );

        glGenBuffers( 1, &self->vbo );
        glGenVertexArrays( 1, &self->vao );
        glGenBuffers( 1, &self->ebo );

        self->shader = shader;
        // self->model -> Pass a position here
        memcpy( &self->model, &GLM_MAT4_IDENTITY, sizeof( GLM_MAT4_IDENTITY ) );
        glm_translate( self->model, pos );

        self->render = SpriteRender;
        self->destroy = SpriteDestroy;

        return self;
}
