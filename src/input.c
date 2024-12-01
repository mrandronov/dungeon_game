#include "input.h"
#include "camera.h"

#include <cglm/cglm.h>

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


