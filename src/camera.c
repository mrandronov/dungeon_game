#include <strings.h>
#include "camera.h"

mat4 view = GLM_MAT4_IDENTITY;
mat4 ortho = GLM_MAT4_IDENTITY;

Camera*
CameraCreate()
{
        Camera* self = ( Camera* ) malloc( sizeof( Camera ) );

        memcpy(&view, &GLM_MAT4_IDENTITY, sizeof(GLM_MAT4_IDENTITY));
        glm_translate(view, (vec3){ 0.0f, 0.0f, -10.0f });

        glm_perspective_default(1.0f, ortho);

        return self;
}
