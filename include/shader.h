#ifndef __SHADER_H__
#define __SHADER_H__

#include <stdbool.h>

typedef enum
{
        VERTEX,
        FRAGMENT
} ShaderType;

typedef enum
{
        MAT4
} UniformType;

int                             ShaderCreate( const char* shader_path, ShaderType type );

typedef struct shader_program_t
{
        int                     id;
        bool                    isActive;

        void                    ( *activate )( struct shader_program_t* self );
        void                    ( *setUniform )( struct shader_program_t* self, char* uniformName, UniformType type, void* data );
        void                    ( *destroy )( struct shader_program_t* self );
} ShaderProgram;

ShaderProgram*                  ShaderProgramCreate( const char* vertx_shdr_path, const char* frag_shdr_path );

#endif /* __SHADER_H__ */
