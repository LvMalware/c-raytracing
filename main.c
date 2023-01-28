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

int help(char * const prog) {
    printf("Usage: %s [options]\n\n", prog);
    printf("Options:\n"
            "   -h              Show this help message and exit\n"
            "   -i              Input .toml file with scene description\n"
            "   -o              Output .bmp file to save rendered scene\n\n");
    return 0;
}

int main(int argc, char *argv[]) {
    char *input_file = "scene.toml";
    char *output_file = "output.bmp";

    char **arg = &argv[1];
    while (*arg) {
        if ((*arg)[0] == '-') {
            switch ((*arg)[1]) {
                case 'h':
                    return help(argv[0]);
                case 'i':
                    input_file = *(++arg);
                    continue;
                case 'o':
                    output_file = *(++arg);
                    continue;
                default:
                    break;
            }
        }
        arg ++;
    }

    if (input_file == NULL || input_file[0] == '-') {
        printf("Missing input file (option -i)\n");
        return 1;
    }

    if (output_file == NULL || output_file[0] == '-') {
        printf("Missing output file (option -o)\n");
        return 1;
    }

    struct scene scene;
    struct bitmap_image image;

    FILE *toml = fopen(input_file, "r");
    if (toml == NULL) {
        printf("Error reading file '%s'\n", input_file);
        return 1;
    }
    scene2toml_read(&scene, toml);
    fclose(toml);

    scene_render(&scene, &image);
    scene_destroy(&scene);
    bitmap_save_file(output_file, &image);
    bitmap_image_destroy(&image);

    return 0;
}
