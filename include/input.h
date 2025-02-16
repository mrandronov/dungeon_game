#ifndef __INPUT_H__
#define __INPUT_H__

#include <stdio.h>
#include <stdbool.h>

#include "glad/glad.h"
#define GL_SILENCE_DEPRECATION
#include <GLFW/glfw3.h>

#include "player.h"

#define KeyInputCount 256

#define MOVESPEED 0.12f

#define DOWN            ( vec3 ){ 0.0f, -MOVESPEED, 0.0f }
#define UP              ( vec3 ){ 0.0f, MOVESPEED, 0.0f }
#define RIGHT           ( vec3 ){ MOVESPEED, -MOVESPEED+0.04, 0.0f }
#define LEFT            ( vec3 ){ -MOVESPEED, MOVESPEED-0.04, 0.0f }
#define ZOOM_OUT        ( vec3 ){ 0.0f, 0.0f, -MOVESPEED }
#define ZOOM_IN         ( vec3 ){ 0.0f, 0.0f, MOVESPEED }


bool            W_KEY;
bool            S_KEY;
bool            A_KEY;
bool            D_KEY;
bool            Q_KEY;
bool            E_KEY;
bool            F_KEY;

bool            UP_KEY;
bool            DOWN_KEY;
bool            LEFT_KEY;
bool            RIGHT_KEY;

bool            T_KEY;

typedef struct key_input_t
{
        bool*   keyFlag;
        int     glfwKey;
} KeyInput;

typedef KeyInput** KeyList; 
KeyList         InputList; 
int             InputListLen;

void            handleInput(Player* player);
void            handlePlayerInput(Player* player);
void            KeyListCreate();

#endif /* __INPUT_H__ */
