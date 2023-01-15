#include <stdio.h>
#include "bitmap.h"

int main(int argc, char *argv[]) {
    struct bitmap_image bitmap;
    bitmap_image_init(&bitmap, 512, 512, 300);
    size_t i;
    for (i = 0; i < 512 * 512; i ++) {
        size_t x = i / 512, y = i % 512;
        if (x > 51 && x < 460 && y > 51 && y < 460) {
            bitmap.pixels[i].r = 0;
            bitmap.pixels[i].g = bitmap.pixels[i].b = 0;
        } else {
            bitmap.pixels[i].r = bitmap.pixels[i].g = bitmap.pixels[i].b = 255;
        }
    }
    bitmap_save_file("output.png", &bitmap);
    bitmap_image_destroy(&bitmap);
    return 0;
}
