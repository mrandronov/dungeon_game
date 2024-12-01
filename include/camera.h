#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "cglm/cglm.h"

extern mat4            view;
extern mat4            ortho;

typedef struct camera_t
{

} Camera;

Camera*         CameraCreate();

#endif /* __CAMERA_H__ */
