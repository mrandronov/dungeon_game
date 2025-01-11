#include <stdbool.h>
#include <stdlib.h>

#include "chest.h"
#include "glad/glad.h"
#define GL_SILENCE_DEPRECATION
#include <GLFW/glfw3.h>
#include <OpenGL/gl.h>

#include "window.h"
#include "logging.h"
#include "input.h"
#include "objects.h"
#include "shader.h"
#include "player.h"
#include "tile_map.h"
#include "camera.h"

bool                    is_game_running = true;
const char*             vertx_shdr_path = "./shaders/vertex.shdr";
const char*             frag_shdr_path = "./shaders/fragment.shdr";
ShaderProgram*          shader_program;
Camera*                 camera;
TileMap*                tile_map;
Player*                 player;
Chest*                  chest;

void
checkKey(int key, int action, int GLFW_KEY, bool* local_key)
{
        if (key == GLFW_KEY)
        {
                if (action == GLFW_PRESS)
                {
                        *local_key = true;
                }
                if (action == GLFW_RELEASE)
                {
                        *local_key = false;
                }
        }
}

void
close_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        {
                is_game_running = false;
        }

        checkKey(key, action, GLFW_KEY_W, &W_KEY);
        checkKey(key, action, GLFW_KEY_S, &S_KEY);
        checkKey(key, action, GLFW_KEY_A, &A_KEY);
        checkKey(key, action, GLFW_KEY_D, &D_KEY);
        checkKey(key, action, GLFW_KEY_Q, &Q_KEY);
        checkKey(key, action, GLFW_KEY_E, &E_KEY);
}

static void
cursor_position_callback(GLFWwindow* window, double mouse_x, double mouse_y)
{
        // TODO
}

void APIENTRY 
GLDebugLogCallback(GLenum source, GLenum type, GLuint id,
   GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
        // TODO: Implement this callback to load on supported platforms.

        // This callback is only available for Opengl 4.3 and greater.
        // MacOS only supports an Opengl implementation up to version 4.1, so this
        // callback cannot be assigned to a glad/OpenGL procedure on Macs.

        char* sevFmt = "";
        char* sourceFmt = "";
        char* typeFmt = "";

        log_debug("OPENGL LOG: [%s:%s:%s]: %s", sevFmt, sourceFmt, typeFmt, message);
}

int
init(Window* self)
{
        if (!glfwInit())
        {
                log_error("GLFW failed to initialize");
                return -1;
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

        self->glfwWindow = glfwCreateWindow(self->width, self->height, "game", NULL, NULL);
        if (!self->glfwWindow)
        {
                log_error("failed to create GLFW window");
                glfwTerminate();
                return -1;
        }

        glfwMakeContextCurrent(self->glfwWindow);

        // Initialize GLAD by loading GL functions
        int result = gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
        if (!result)
        {
                log_error("failed to initialize GLAD: %d", result);
                return -1;
        }

        // Callbacks 
        glfwSetKeyCallback(self->glfwWindow, close_key_callback);
        glfwSetCursorPosCallback(self->glfwWindow, cursor_position_callback);

        // Initialize global object list that we will render everything from.
        ObjectListCreate();

        // Local Objects
        camera = CameraCreate();
        shader_program = ShaderProgramCreate(vertx_shdr_path, frag_shdr_path);
        tile_map = TileMapCreate(shader_program);
        player = PlayerCreate(shader_program);
        chest = ChestCreate(shader_program);

        log_info("window initialization: success");

        return 0;

}

void
render(Window* self)
{
        glClearColor(0.04f, 0.054f, 0.078f, 0.5f);
        glClear(GL_COLOR_BUFFER_BIT);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        RenderObjects(); 
        
        glfwSwapBuffers(self->glfwWindow);
}

void
run(Window* self) {
        self->isRunning = true;

        float           last_frame_time = 0.0f;
        while (!glfwWindowShouldClose(self->glfwWindow) && is_game_running)
        {
                float           now = glfwGetTime();

                glfwPollEvents();
                if ((now - last_frame_time) >= (1.0f / self->fpsCap))
                {
                        handleInput();
                        handlePlayerInput(player);
                        render(self);

                        last_frame_time = now;
                }
        }

        self->isRunning = false;

        glfwDestroyWindow(self->glfwWindow);
        glfwTerminate();
}

Window*
WindowCreate(int width, int height, int fpsCap) {
        Window* self = (Window*) malloc(sizeof(Window));

        self->width = width;
        self->height = height;
        self->fpsCap = fpsCap;

        return self;
}
