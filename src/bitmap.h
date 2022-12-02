#ifndef __BITMAP_H
#define __BITMAP_H

#include <stdint.h>

typedef struct BITMAP {
    uint8_t width;
    uint8_t height;
    const unsigned char* data_pgm;
} BITMAP;

#endif