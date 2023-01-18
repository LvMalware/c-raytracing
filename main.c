#include <stdio.h>
#include "bitmap.h"
#include "raycast.h"

int main(int argc, char *argv[]) {
    struct scene scene;
    struct bitmap_image image;
    scene_create(&scene, 1080, 1080);
    scene.light.position = (struct point) { 1, 1, 9 };
    scene.camera = (struct point) { -1, 1, 5 };
    scene.max_depth = 5;

    /* scene_object(&scene, &(struct object) {
        .type = OBJECT_SPHERE,
        .center = { .x = -0.2, .y = 0, .z = -0.5 },
        .radius = 0.7,
        .ambient = { .r = 0.7, .g = 0, .b = 0 },
        .diffuse = { .r = 0.7, .g = 0, .b = 0 },
        .specular = { .r = 1, .g = 1, .b = 1 },
        .shineness = 100,
        .reflection = 0.5
    });

    scene_object(&scene, &(struct object) {
        .type = OBJECT_SPHERE,
        .center = { .x = -0.22, .y = -0.25, .z = 0.4 },
        .radius = 0.15,
        .ambient = { .r = 0, .g = 0.6, .b = 0 },
        .diffuse = { .r = 0, .g = 0.7, .b = 0 },
        .specular = { .r = 1, .g = 1, .b = 1 },
        .shineness = 100,
        .reflection = 0.5
    });

    scene_object(&scene, &(struct object) {
        .type = OBJECT_SPHERE,
        .center = { .x = 0.7, .y = -0.2, .z = -0.4 },
        .radius = 0.25,
        .ambient = { .r = 0, .g = 0.6, .b = .6 },
        .diffuse = { .r = 0, .g = 0.7, .b = .7 },
        .specular = { .r = 1, .g = 1, .b = 1 },
        .shineness = 100,
        .reflection = 0.5
    }); */

    scene_object(&scene, &(struct object) {
        .type = OBJECT_SPHERE,
        .center = { .x = 0.91, .y = 0.2, .z = -1.2 },
        .radius = 0.15,
        .ambient = { .r = 0.0, .g = 0.0, .b = 0.7 },
        .diffuse = { .r = 0.0, .g = 0.0, .b = 0.7 },
        .specular = { .r = 1.0, .g = 1.0, .b = 1.0 },
        .shineness = 100,
        .reflection = 0.3
    });

    scene_object(&scene, &(struct object) {
        .type = OBJECT_SPHERE,
        .center = { .x = 0.83, .y = 0.6, .z = -0.9 },
        .radius = 0.15,
        .ambient = { .r = 0.7, .g = 0.0, .b = 0.0 },
        .diffuse = { .r = 0.7, .g = 0.0, .b = 0.0 },
        .specular = { .r = 1.0, .g = 1.0, .b = 1.0 },
        .shineness = 100,
        .reflection = 0.3
    });

    scene_object(&scene, &(struct object) {
        .type = OBJECT_SPHERE,
        .center = { .x = 0.5, .y = 0.7, .z = -0.5 },
        .radius = 0.15,
        .ambient = { .r = 0.7, .g = 0.7, .b = 0.0 },
        .diffuse = { .r = 0.7, .g = 0.7, .b = 0.0 },
        .specular = { .r = 1.0, .g = 1.0, .b = 1.0 },
        .shineness = 100,
        .reflection = 0.3
    });

    scene_object(&scene, &(struct object) {
        .type = OBJECT_SPHERE,
        .center = { .x = -0.4, .y = 0, .z = 0.2 },
        .radius = 0.22,
        .ambient = { .r = 0.7, .g = 0.7, .b = 0.7 },
        .diffuse = { .r = 0.7, .g = 0.7, .b = 0.0 },
        .specular = { .r = 1.0, .g = 1.0, .b = 1.0 },
        .shineness = 100,
        .reflection = 0.3
    });

    scene_object(&scene, &(struct object) {
        .type = OBJECT_SPHERE,
        .center = { .x = -0.55, .y = -0.2, .z = 0.4 },
        .radius = 0.02,
        .ambient = { .r = 0, .g = 0, .b = 0 },
        .diffuse = { .r = 0, .g = 0, .b = 0.0 },
        .specular = { .r = 1.0, .g = 1.0, .b = 1.0 },
        .shineness = 300,
        .reflection = 0
    });

    scene_object(&scene, &(struct object) {
        .type = OBJECT_SPHERE,
        .center = { .x = -0.56, .y = -0.21, .z = 0.41 },
        .radius = 0.02,
        .ambient = { .r = 0.42, .g = 0.14, .b = 0 },
        .diffuse = { .r = 0.42, .g = 0.14, .b = 0.0 },
        .specular = { .r = 1.0, .g = 1.0, .b = 1.0 },
        .shineness = 300,
        .reflection = 0
    });

    scene_object(&scene, &(struct object) {
        .type = OBJECT_SPHERE,
        .center = { .x = -0.561, .y = -0.211, .z = 0.411 },
        .radius = 0.02,
        .ambient = { .r = 0.42, .g = 0.14, .b = 0 },
        .diffuse = { .r = 0.42, .g = 0.14, .b = 0.0 },
        .specular = { .r = 1.0, .g = 1.0, .b = 1.0 },
        .shineness = 300,
        .reflection = 0
    });

    scene_object(&scene, &(struct object) {
        .type = OBJECT_SPHERE,
        .center = { .x = -0.562, .y = -0.212, .z = 0.412 },
        .radius = 0.02,
        .ambient = { .r = 0.42, .g = 0.14, .b = 0 },
        .diffuse = { .r = 0.42, .g = 0.14, .b = 0.0 },
        .specular = { .r = 1.0, .g = 1.0, .b = 1.0 },
        .shineness = 300,
        .reflection = 0
    });

    int i;
    double add = 0.001;
    double adr = 0.00001;
    for (i = 0; i < 500; i ++) {
        scene_object(&scene, &(struct object) {
            .type = OBJECT_SPHERE,
            .center = { .x = -(0.563 + add), .y = -(0.213 + add), .z = (0.413 + add) },
            .radius = 0.02 + adr,
            .ambient = { .r = 0.42, .g = 0.14, .b = 0 },
            .diffuse = { .r = 0.42, .g = 0.14, .b = 0.0 },
            .specular = { .r = 1.0, .g = 1.0, .b = 1.0 },
            .shineness = 300,
            .reflection = 0
        });
        add += 0.001;
        adr += 0.00001;
    }

    scene_object(&scene, &(struct object) {
        .type = OBJECT_PLANE,
        .center = { .x = -0.8, .y = 0, .z = 0.2 },
        .normal = { 1, -0.5, 1 },
        .ambient = { .r = 0, .g = .9, .b = 0.0 },
        .diffuse = { .r = 0, .g = 0.8, .b = 0.0 },
        .specular = { .r = 1.0, .g = 1.0, .b = 1.0 },
        .shineness = 200,
        .reflection = 0
    });

    scene_object(&scene, &(struct object) {
        .type = OBJECT_CUBE,
        .height = 0.15,
        .center =  { .x = 0.7, .y = -0.9, .z = -1.45 },
        .ambient = { .r = 0.42, .g = .14, .b = 0.0 },
        .diffuse = { .r = 0.42, .g = .14, .b = 0.0 },
        .specular = { .r = 1.0, .g = 1.0, .b = 1.0 },
        .shineness = 100,
        .reflection = 0.1
    });

    scene_object(&scene, &(struct object) {
        .type = OBJECT_CUBE,
        .height = 0.15,
        .center =  { .x = 0.7, .y = -0.9, .z = -1.43 },
        .ambient = { .r = 0.7, .g = .8, .b = 0.7 },
        .diffuse = { .r = 0.7, .g = .9, .b = 0.0 },
        .specular = { .r = 1.0, .g = 1.0, .b = 1.0 },
        .shineness = 100,
        .reflection = 0
    });

    scene_render(&scene, &image);
    scene_destroy(&scene);
    bitmap_save_file("output.bmp", &image);
    bitmap_image_destroy(&image);
    return 0;
}
