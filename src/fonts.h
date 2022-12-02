#ifndef __FONTS_H
#define __FONTS_H

#include <stdint.h>

typedef struct FONT_CHAR_INFO {
    uint8_t width;
    uint16_t offset;
} FONT_CHAR_INFO;

typedef struct FONT {
    uint8_t height;
    char start_char;
    uint8_t letter_space;
    FONT_CHAR_INFO* char_info;
    uint8_t* bitmaps;
} FONT;

// ---------Fonts----------

extern FONT FontSegoeUI36;

#endif