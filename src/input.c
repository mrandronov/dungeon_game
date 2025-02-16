#include <cglm/cglm.h>

#include "input.h"
#include "camera.h"

void
handleInput(Player* player)
{
        handlePlayerInput(player);
        if ( UP_KEY )
        {
                glm_translate( view, DOWN );
        }

        if ( DOWN_KEY )
        {
                glm_translate( view, UP );
        }

        if ( LEFT_KEY )
        {
                glm_translate( view, RIGHT );
        }

        if ( RIGHT_KEY )
        {
                glm_translate( view, LEFT );
        }

        if ( Q_KEY )
        {
                glm_translate( view, ZOOM_OUT );
        }

        if ( E_KEY )
        {
                glm_translate( view, ZOOM_IN );
        }
}

// Use vectors instead of flat directional movement.
void
handlePlayerInput(Player* player)
{
        if ( W_KEY )
        {
                player->move(player, UP);
        }

        if ( S_KEY )
        {
                player->move(player, DOWN);
        }

        if ( A_KEY )
        {
                player->move(player, LEFT);
                player->isFacingRight = false;
        }

        if ( D_KEY )
        {
                player->move(player, RIGHT);
                player->isFacingRight = true;
        }

        if ( F_KEY )
        {
                player->placeTorch(player);
        }

        if ( W_KEY || S_KEY || A_KEY || D_KEY ) {
                player->currentTexture = player->runTexture;
        } else {
                player->currentTexture = player->idleTexture;
        }
        player->currentTexture->isStarted = true;
}

void
addKeyInput(bool* keyFlag, int glfwKey) {
        KeyInput*       input = malloc(sizeof(KeyInput));

        input->keyFlag = keyFlag;
        input->glfwKey = glfwKey;

        InputList[InputListLen] = input;
        InputListLen++;
}

void
KeyListCreate()
{
        InputList = malloc(sizeof(KeyInput) * KeyInputCount); 
        InputListLen = 0;

        addKeyInput(&W_KEY, GLFW_KEY_W);
        addKeyInput(&A_KEY, GLFW_KEY_A);
        addKeyInput(&S_KEY, GLFW_KEY_S);
        addKeyInput(&D_KEY, GLFW_KEY_D);
        addKeyInput(&Q_KEY, GLFW_KEY_Q);
        addKeyInput(&E_KEY, GLFW_KEY_E);
        addKeyInput(&F_KEY, GLFW_KEY_F);

        addKeyInput(&UP_KEY, GLFW_KEY_UP);
        addKeyInput(&DOWN_KEY, GLFW_KEY_DOWN);
        addKeyInput(&LEFT_KEY, GLFW_KEY_LEFT);
        addKeyInput(&RIGHT_KEY, GLFW_KEY_RIGHT);
}

