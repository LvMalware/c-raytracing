#ifndef __BITMAP_H
#define __BITMAP_H

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

struct rgb_data {
    uint8_t b;
    uint8_t g;
    uint8_t r;
} __attribute__((packed));

static_assert(sizeof(struct rgb_data) == 3, "invalid struct rgb_data size");

struct bitmap_file_header {
    uint8_t  type[2];
    uint32_t file_size;
    uint32_t reserved;
    uint32_t data_offset;
} __attribute__((packed));

struct bitmap_image_header {
    uint32_t size;
    uint32_t width;
    uint32_t height;
    uint16_t planes;
    uint16_t pixel_bits;
    uint32_t compression;
    uint32_t data_size;
    uint32_t hresolution;
    uint32_t vresolution;
    uint32_t used_colors;
    uint32_t important_colors;
} __attribute__((packed));

struct bitmap_image {
    struct bitmap_file_header file_header;
    struct bitmap_image_header image_header;
    struct rgb_data *pixels;
};

void bitmap_image_destroy(struct bitmap_image * const);
void bitmap_save_file(const char * const, struct bitmap_image * const);
void bitmap_image_init(struct bitmap_image * const, const uint32_t,
                       const uint32_t, const uint32_t);
void bitmap_image_header_init(struct bitmap_image_header * const,
                              const uint32_t, const uint32_t, const uint32_t);
void bitmap_file_header_init(struct bitmap_file_header * const, const uint32_t);


#endif /* bitmap.h */
