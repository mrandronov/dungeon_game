#include <strings.h>

#include "camera.h"

float zoom = -20.0f;
mat4 view = GLM_MAT4_IDENTITY;
mat4 ortho = GLM_MAT4_IDENTITY;
float mouseX = 0.0f;
float mouseY = 0.0f;

void
CameraSetPos(Camera* self, vec3 pos)
{
        view[3][0] = -pos[0];
        view[3][1] = -pos[1];
}

Camera*
CameraCreate()
{
        Camera* self = ( Camera* ) malloc(sizeof(Camera));

        memcpy(&view, &GLM_MAT4_IDENTITY, sizeof(GLM_MAT4_IDENTITY));

        /*glm_perspective_default(1.0f, ortho);*/
        /*glm_ortho_default(1.0f, ortho);*/
        
        float   div             = 8.0f;
        int     viewPortWidth   = 1200;
        int     viewPortHeight  = 675;

        float   left      =        (float) viewPortWidth / div;
        float   right     =       -(float) viewPortWidth / div;
        float   down      =        (float) viewPortHeight / div;
        float   up        =       -(float) viewPortHeight / div;

        glm_ortho(
                        left / zoom,
                        right / zoom,
                        down / zoom,
                        up / zoom,
                        0.1f,
                        100.0f,
                        ortho
        );

        glm_lookat(
                (vec3){0.0f, 3.0f, 3.0f},
                (vec3){2.0f, 0.0f, 0.0f},
                (vec3){0.0f, 1.0f, 0.0f},
                view
        );

        glm_translate(view, ( vec3 ){ 0.0f, 0.0f, -5.0f});

        return self;
}
