#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define GL_SILENCE_DEPRECATION
#include "glad/glad.h"
#include <OpenGL/gl.h>

#include "logging.h"
#include "shader.h"

char*
ReadFileName(const char* filename)
{
        FILE*                   file_handler = fopen(filename, "r");

        if (!file_handler)
        {
                log_error("error reading shader file (%s): %s\n", filename, strerror(errno));
                exit(1);
        }

        fseek(file_handler, 0, SEEK_END);

        int                     string_size = ftell(file_handler);

        rewind(file_handler);

        char*                   buffer = (char*) malloc(sizeof(char) * (string_size + 1));
        int                     read_size = fread(buffer, sizeof(char), string_size, file_handler);

        buffer[ string_size ] = '\0';

        if (string_size != read_size)
        {
                log_error("error reading shader file (%s): file size is %d bytes but only read %d bytes.\n", filename, string_size, read_size);
                free(buffer);
                buffer = NULL;
        }

        fclose(file_handler);

        return buffer;
}

int
ShaderCreate(const char* filename, ShaderType type)
{
        const char*             source = ReadFileName(filename);

        GLenum                  shader_type;
        if (type == VERTEX)
        {
                shader_type = GL_VERTEX_SHADER;
        }
        else
        {
                shader_type = GL_FRAGMENT_SHADER;
        }

        int                     id = glCreateShader(shader_type);

        // Compile the shader
        glShaderSource(id, 1, &source, NULL);
        glCompileShader(id);

        free((char*) source);

        int             success;
        char            info_log[ 512 ];

        glGetShaderiv(id, GL_COMPILE_STATUS, &success);

        if (!success)
        {
                glGetShaderInfoLog(id, 512, NULL, info_log);
                log_error("failed to compile shader from %s: %s\n", filename, info_log);
                return 0;
        }

        log_info("create shader: success for %s", filename);

        return id;
}

void
ShaderProgramActivate(ShaderProgram* self)
{
        if (!self->isActive) {
                glUseProgram(self->id);
                self->isActive = true;
        }
}

void
ShaderSetMat4(ShaderProgram* self, char* uniformName, float* mat)
{
        int             uniformLoc = glGetUniformLocation(self->id, uniformName);
        glUniformMatrix4fv(uniformLoc, 1, GL_FALSE, mat);
}

void
ShaderSetUniform(ShaderProgram* self, char* uniformName, UniformType type, void* data)
{
        if (type != MAT4) {
                log_error("[ shader ID: %d ] unrecognized UniformType for %s\n", self->id, uniformName);
                return;
        }
        
        ShaderSetMat4(self, uniformName, (float*) data);
}

void
ShaderProgramDestroy(ShaderProgram* self)
{
        glDeleteProgram(self->id);

        free(self);
        self = NULL;
}

ShaderProgram*
ShaderProgramCreate(const char* vertx_shdr_path, const char* frag_shdr_path)
{
        ShaderProgram*          self = (ShaderProgram*) malloc(sizeof(ShaderProgram));

        log_info("creating shader program with shaders: (%s) (%s)", vertx_shdr_path, frag_shdr_path);

        int                     vertex_shader = ShaderCreate(vertx_shdr_path, VERTEX);
        int                     fragment_shader = ShaderCreate(frag_shdr_path, FRAGMENT);

        self->id = glCreateProgram();
        self->isActive = false;

        // attach shaders to shader program
        glAttachShader(self->id, vertex_shader);
        glAttachShader(self->id, fragment_shader);

        // link the shader program so that the GPU knows where it is
        glLinkProgram(self->id);

        // Free vertex and fragment shaders since they are no longer needed
        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);

        // check for failures while linking the program
        int                     success;
        char                    info_log[ 512 ];

        glGetProgramiv(self->id, GL_LINK_STATUS, &success);
        if (!success)
        {
                glGetProgramInfoLog(self->id, 512, NULL, info_log);
                log_error("failed to link shader program: %s\n", info_log);
                exit(1);
        }

        self->activate = ShaderProgramActivate;
        self->setUniform = ShaderSetUniform;
        self->destroy = ShaderProgramDestroy;

        log_info("shader program creation: success");

        return self;
}

