#ifndef __RAYCAST_H
#define __RAYCAST_H

#include <math.h>
#include <stdlib.h>

#include "bitmap.h"
#include "objects.h"

struct scene {
    int width;
    int height;
    double ratio;
    int max_depth;
    double screen[4];
    struct light light;
    struct point camera;
    size_t object_count;
    struct object *objects;
};

void scene_create(struct scene * const, int, int);
void scene_object(struct scene * const, struct object * const);
void scene_render(struct scene * const, struct bitmap_image * const);
void scene_destroy(struct scene * const);

#endif /* raycast.h */
