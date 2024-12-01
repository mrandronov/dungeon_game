#ifndef __INPUT_H__
#define __INPUT_H__

#include <stdio.h>
#include <stdbool.h>

#include "glad/glad.h"
#define GL_SILENCE_DEPRECATION
#include <GLFW/glfw3.h>

#define MOVESPEED 0.5f
#define DOWN ( vec3 ){ 0.0f, -MOVESPEED, 0.0f }
#define UP ( vec3 ){ 0.0f, MOVESPEED, 0.0f }
#define RIGHT ( vec3 ){ MOVESPEED, 0.0f, 0.0f }
#define LEFT ( vec3 ){ -MOVESPEED, 0.0f, 0.0f }
#define ZOOM_OUT ( vec3 ){ 0.0f, 0.0f, -MOVESPEED }
#define ZOOM_IN ( vec3 ){ 0.0f, 0.0f, MOVESPEED }


bool            W_KEY;
bool            S_KEY;
bool            A_KEY;
bool            D_KEY;
bool            Q_KEY;
bool            E_KEY;

void            handleInput();

#endif /* __INPUT_H__ */
