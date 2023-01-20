/*
 * Copyright 2023 Lucas V. Araujo (https://github.com/lvmalware)
 *
 * This file is part of c-raytracing.
 * c-raytracing is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * c-raytracing is distributed in the hope that it will be useful,but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this file. If not, see <https://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include "bitmap.h"
#include "raycast.h"

int main(int argc, char *argv[]) {
    struct scene scene;
    struct bitmap_image image;
    scene_create(&scene, 1080, 1080);
    scene.light.position = (struct point) { 0.5, 0.5, 9 };
    scene.camera = (struct point) { 1.5, 1, 4 };
    scene.max_depth = 5;

    scene_object(&scene, &(struct object) {
        .type = OBJECT_PLANE,
        .normal = { -0.5, -0.5, 1 },
        .center = { .x = -0.8, .y = 0, .z = 0.2 },
        .ambient = { .r = 0, .g = .9, .b = 0.0 },
        .diffuse = { .r = 0, .g = 0.8, .b = 0.0 },
        .specular = { .r = 1.0, .g = 1.0, .b = 1.0 },
        .shineness = 600,
        .reflection = 0
    });

    scene_object(&scene, &(struct object) {
        .type = OBJECT_CYLINDER,
        .height = 0.8,
        .radius = 0.025,
        .center = { 0, 0.5, 1 },
        .ambient = { .r = 1, .g = 0.14, .b = 0 },
        .diffuse = { .r = 0.42, .g = 0.14, .b = 0 },
        .specular = { .r = 1.0, .g = 1.0, .b = 1.0 },
        .shineness = 1000,
        .reflection = 0 
    });

    scene_object(&scene, &(struct object) {
        .type = OBJECT_SPHERE,
        .center = { .x = 0.15, .y = 0.54, .z = 1.02 },
        .radius = 0.06,
        .ambient = { .r = 0.7, .g = 0.7, .b = 0.7 },
        .diffuse = { .r = 0.7, .g = 0.7, .b = 0.0 },
        .specular = { .r = 1.0, .g = 1.0, .b = 1.0 },
        .shineness = 100,
        .reflection = 0.3
    });

    scene_object(&scene, &(struct object) {
        .type = OBJECT_SPHERE,
        .center = { .x = 0.89, .y = 0.6, .z = 1.4 },
        .radius = 0.05,
        .ambient = { .r = 0.7, .g = 0.0, .b = 0.0 },
        .diffuse = { .r = 0.7, .g = 0.0, .b = 0.0 },
        .specular = { .r = 1.0, .g = 1.0, .b = 1.0 },
        .shineness = 100,
        .reflection = 0.1
    });

    scene_object(&scene, &(struct object) {
        .type = OBJECT_SPHERE,
        .center = { .x = 0.78, .y = 0.7, .z = 1.4 },
        .radius = 0.05,
        .ambient = { .r = 0.0, .g = 0.0, .b = 0.7 },
        .diffuse = { .r = 0.0, .g = 0.0, .b = 0.7 },
        .specular = { .r = 1.0, .g = 1.0, .b = 1.0 },
        .shineness = 100,
        .reflection = 0.1
    });

    scene_object(&scene, &(struct object) {
        .type = OBJECT_SPHERE,
        .center = { .x = 0.8, .y = 0.3, .z = 1.2 },
        .radius = 0.05,
        .ambient = { .r = 0.7, .g = 0.7, .b = 0.0 },
        .diffuse = { .r = 0.7, .g = 0.7, .b = 0.0 },
        .specular = { .r = 1.0, .g = 1.0, .b = 1.0 },
        .shineness = 100,
        .reflection = 0.3
    });


    scene_object(&scene, &(struct object) {
        .type = OBJECT_CUBE,
        .height = 0.05,
        .center =  { .x = 0.2, .y = -0.2, .z = 0.65 },
        .ambient = { .r = 0.42, .g = .14, .b = 0.0 },
        .diffuse = { .r = 0.42, .g = .14, .b = 0.0 },
        .specular = { .r = 1.0, .g = 1.0, .b = 1.0 },
        .shineness = 100,
        .reflection = 0.1
    });

    scene_object(&scene, &(struct object) {
        .type = OBJECT_CUBE,
        .height = 0.05,
        .center =  { .x = 0.2, .y = -0.2, .z = 0.651 },
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
