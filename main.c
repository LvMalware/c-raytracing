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

#include "toml.h"
#include "bitmap.h"
#include "raycast.h"

int main(int argc, char *argv[]) {
    struct scene scene;
    struct bitmap_image image;

    FILE *toml = fopen("scene.toml", "r");
    scene2toml_read(&scene, toml);
    fclose(toml);

    scene_render(&scene, &image);
    scene_destroy(&scene);
    bitmap_save_file("output.bmp", &image);
    bitmap_image_destroy(&image);

    return 0;
}
