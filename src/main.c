#include <stdio.h>
#include <stdbool.h>

#include "glad/glad.h"
#include "texture_atlas.h"
#define GL_SILENCE_DEPRECATION
#include <GLFW/glfw3.h>
#include <OpenGL/gl.h>

#include <cglm/cglm.h>

#include "input.h"
#include "shader.h"
#include "tile_map.h"

#include "logging.h"

GLFWwindow*             window;
int                     screen_width = 1100;
int                     screen_height = 900;
bool                    is_game_running = true;
float                   fps_cap = 1000.0f / 60.0f;
double                  mouse_x = 0.0f;
double                  mouse_y = 0.0f;

unsigned int            VBO;
unsigned int            VAO;
unsigned int            EBO;

const char*             vertx_shdr_path = "./shaders/vertex.shdr";
const char*             frag_shdr_path = "./shaders/fragment.shdr";
ShaderProgram*          shader_program;

char*                   texture_atlas_path = "./res/Textures.png";
TextureAtlas*           textureAtlas;

TileMap*                tile_map;

void
checkKey( int key, int action, int GLFW_KEY, bool* local_key )
{
        if ( key == GLFW_KEY )
        {
                if ( action == GLFW_PRESS )
                {
                        *local_key = true;
                }
                if ( action == GLFW_RELEASE )
                {
                        *local_key = false;
                }
        }
}

void
close_key_callback( GLFWwindow* window, int key, int scancode, int action, int mods )
{
        if ( key == GLFW_KEY_ESCAPE && action == GLFW_PRESS )
        {
                is_game_running = false;
        }

        checkKey( key, action, GLFW_KEY_W, &W_KEY );
        checkKey( key, action, GLFW_KEY_S, &S_KEY );
        checkKey( key, action, GLFW_KEY_A, &A_KEY );
        checkKey( key, action, GLFW_KEY_D, &D_KEY );
        checkKey( key, action, GLFW_KEY_Q, &Q_KEY );
        checkKey( key, action, GLFW_KEY_E, &E_KEY );
}

static void
cursor_position_callback( GLFWwindow* window, double mouse_x, double mouse_y )
{
        // TODO
}

int
window_init()
{
        if ( !glfwInit() )
        {
                log_error( "GLFW failed to initialize" );
                return -1;
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        window = glfwCreateWindow( screen_width, screen_height, "Hello world", NULL, NULL );
        if ( !window )
        {
                log_error( "failed to created GLFW window" );
                glfwTerminate();
                return -1;
        }

        glfwMakeContextCurrent( window );

        // Initialize GLAD by loading GL functions
        int result = gladLoadGLLoader( ( GLADloadproc ) glfwGetProcAddress );
        if ( !result )
        {
                log_error( "failed to initialize GLAD: %d", result );
                return -1;
        }

        // Callbacks 
        glfwSetKeyCallback( window, close_key_callback );
        glfwSetCursorPosCallback( window, cursor_position_callback );

        return 0;
}

void
render()
{
        glClearColor( 0.11f, 0.11f, 0.11f, 1.0f );
        glClear( GL_COLOR_BUFFER_BIT );
        
        tile_map->render( tile_map );
        
        glfwSwapBuffers( window );
}

int
main( int argc, char* argv[] )
{
        if ( window_init() != 0 )
        {
                return -1;
        }

        log_debug("initialization complete, creating resources and running...");

        textureAtlas = TextureAtlasCreate( texture_atlas_path );
        shader_program = ShaderProgramCreate( vertx_shdr_path, frag_shdr_path );
        tile_map = TileMapCreate( shader_program, textureAtlas );

        float           last_frame_time = 0.0f;
        while ( !glfwWindowShouldClose( window ) && is_game_running )
        {
                float           now = glfwGetTime();

                glfwPollEvents();
                if ( ( now - last_frame_time ) >= ( 1.0f / fps_cap ) )
                {
                        handleInput();
                        render();

                        last_frame_time = now;
                }
        }

        glfwDestroyWindow( window );
        glfwTerminate();

        shader_program->destroy( shader_program );

        return 0;
}
