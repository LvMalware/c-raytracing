#include "bitmap.h"

void bitmap_file_header_init(struct bitmap_file_header * const header, const uint32_t file_size) {
    memcpy(header->type, "BM", 2);
    header->reserved = 0;
    header->data_offset = 0; //sizeof(*header) + sizeof(struct bitmap_image_header);
    header->file_size = header->data_offset + file_size;
}

void bitmap_image_header_init(struct bitmap_image_header * const header,
                              const uint32_t width, const uint32_t height,
                              const uint32_t dpi) {

    uint32_t ppm = (double) dpi * 39.275;
    uint32_t file_size = width * height * 3;

    header->size = sizeof(*header);
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
    fwrite(&(bitmap->image_header), 1, sizeof(bitmap->image_header), output);
    fwrite(bitmap->pixels, sizeof(struct rgb_data), size, output);
    fclose(output);
}
