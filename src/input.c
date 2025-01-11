#include <cglm/cglm.h>

#include "input.h"
#include "camera.h"


void
handleInput()
{
        if ( W_KEY )
        {
                glm_translate( view, DOWN );
        }

        if ( S_KEY )
        {
                glm_translate( view, UP );
        }

        if ( A_KEY )
        {
                glm_translate( view, RIGHT );
        }

        if ( D_KEY )
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
                glm_translate( player->object->model, UP );
        }

        if ( S_KEY )
        {
                glm_translate( player->object->model, DOWN );
        }

        if ( A_KEY )
        {
                glm_translate( player->object->model, LEFT );
                player->isFacingRight = false;
        }

        if ( D_KEY )
        {
                glm_translate( player->object->model, RIGHT );
                player->isFacingRight = true;
        }

        if ( W_KEY || S_KEY || A_KEY || D_KEY ) {
                player->currentTexture = player->runTexture;
        } else {
                player->currentTexture = player->idleTexture;
        }
        player->currentTexture->isStarted = true;
}

