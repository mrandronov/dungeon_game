#include <stdlib.h>

#include "objects.h"

void
RenderObjects()
{
        for (int i = 0; i < ObjectCount; i++)
        {
                RenderObject* obj = Objects[ i ];
                obj->render(obj);
        }
}

void
AddObject(RenderObject* objPtr)
{
        Objects[ObjectCount] = objPtr;
        ObjectCount++;
}

ObjectList
ObjectListCreate()
{
        Objects         = malloc(MAX_OBJECTS * sizeof(RenderObject*));
        ObjectCount     = 0;

        return Objects;
}
