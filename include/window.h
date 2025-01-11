#ifndef __WINDOW_H__
#define __WINDOW_H__

#include <stdbool.h>

#include "GLFW/glfw3.h"

typedef struct window_t
{
        GLFWwindow*             glfwWindow;
        int                     width;
        int                     height;
        float                   fpsCap;
        bool                    isRunning;
} Window;


int             init(Window* self);
void            run(Window* self);
Window*         WindowCreate(int width, int height, int fpsCap);

#endif /* __WINDOW_H__ */
