#ifndef __OBJECT_H__
#define __OBJECT_H__

#include "renderer.h"

// TODO: This value should eventually be in the 1000's
#define MAX_OBJECTS     256 

typedef RenderObject**  ObjectList;

ObjectList              Objects;
int                     ObjectCount;

void                    RenderObjects();
void                    AddObject(RenderObject* objPtr);
ObjectList              ObjectListCreate();

#endif /* __OBJECT_H__ */
