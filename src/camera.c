#include "camera.h"

mat4 view = GLM_MAT4_IDENTITY;
mat4 ortho = GLM_MAT4_IDENTITY;

Camera*
CameraCreate()
{
        Camera* self = ( Camera* ) malloc( sizeof( Camera ) );
        return self;
}
