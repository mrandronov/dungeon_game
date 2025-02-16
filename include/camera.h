#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "cglm/cglm.h"

extern float           zoom;
extern mat4            view;
extern mat4            ortho;
extern float           mouseX;
extern float           mouseY;

typedef struct camera_t
{
        // TODO: Eventually make the camera an interesting component.
} Camera;

void            CameraSetPos(Camera* self, vec3 pos);
Camera*         CameraCreate();

#endif /* __CAMERA_H__ */
