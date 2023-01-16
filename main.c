#include <stdio.h>
#include "bitmap.h"
#include "raycast.h"

int main(int argc, char *argv[]) {
    struct scene scene;
    struct bitmap_image image;
    scene_create(&scene, 512, 512);
    scene.light.position = (struct point) { 5, 5, 7 };

    scene_object(&scene, &(struct object) {
        .type = OBJECT_SPHERE,
        .center = { .x = -0.2, .y = 0, .z = -1 },
        .radius = 0.7,
        .ambient = { .r = 0.7, .g = 0, .b = 0 },
        .diffuse = { .r = 0.7, .g = 0, .b = 0 },
        .specular = { .r = 1, .g = 1, .b = 1 },
        .shineness = 100,
        .reflection = 0.5
    });

    scene_object(&scene, &(struct object) {
        .type = OBJECT_SPHERE,
        .center = { .x = 0.5, .y = 0, .z = 0 },
        .radius = 0.15,
        .ambient = { .r = 0, .g = 0.1, .b = 0 },
        .diffuse = { .r = 0, .g = 0.6, .b = 0 },
        .specular = { .r = 1, .g = 1, .b = 1 },
        .shineness = 100,
        .reflection = 0.5
    });

    scene_render(&scene, &image);
    scene_destroy(&scene);
    bitmap_save_file("output.bmp", &image);
    bitmap_image_destroy(&image);
    return 0;
}
