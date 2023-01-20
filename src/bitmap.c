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

#include "bitmap.h"

void bitmap_file_header_init(struct bitmap_file_header * const header, const uint32_t file_size) {
    memcpy(header->type, "BM", 2);
    header->reserved = 0;
    header->data_offset = 54;
    header->file_size = file_size;
}

void bitmap_image_header_init(struct bitmap_image_header * const header,
                              const uint32_t width, const uint32_t height,
                              const uint32_t dpi) {

    uint32_t ppm = dpi * 39.275;
    uint32_t file_size = 54 + sizeof(struct rgb_data) * width * height;
    header->size = 40;
    header->width = width;
    header->height = height;
    header->planes = 1;
    header->data_size = file_size;
    header->pixel_bits = 24;
    header->compression = 0;
    header->hresolution = ppm;
    header->vresolution = ppm;
    header->used_colors = 0;
    header->important_colors = 0;
}

void bitmap_image_init(struct bitmap_image * const bitmap, const uint32_t width,
                       const uint32_t height, const uint32_t dpi) {
    bitmap_image_header_init(&(bitmap->image_header), width, height, dpi);
    bitmap_file_header_init(&(bitmap->file_header),
                            bitmap->image_header.data_size);
    bitmap->pixels = calloc(width * height, sizeof(bitmap->pixels[0]));
}

void bitmap_image_destroy(struct bitmap_image * const bitmap) {
    if (bitmap == NULL) return;
    free(bitmap->pixels);
    bitmap->pixels = NULL;
}

void bitmap_save_file(const char * const filename, struct bitmap_image * const bitmap) {
    FILE *output = fopen(filename, "wb");
    if (!output) return;
    uint32_t size = bitmap->image_header.width * bitmap->image_header.height;
    fwrite(&(bitmap->file_header), 1, 14, output);
    fwrite(&(bitmap->image_header), 1, 40, output);
    fwrite(bitmap->pixels, sizeof(struct rgb_data), size, output);
    fclose(output);
}
