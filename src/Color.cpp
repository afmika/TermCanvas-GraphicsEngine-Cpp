#include "Color.h"

Color::Color(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b) {
    this->r = r;
    this->g = g;
    this->b = b;
}

Color::Color() { }

// 3 * 8 == 24 < 32
uint32_t Color::toUint32 () {
    uint32_t result = 0x000000;
    result  |= r << 16;
    result  |= g <<  8;
    result  |= g <<  0;
    return result;
}

char Color::toChar (std::string letters) {
    double  color   = (double) r;
            color  += (double) g,
            color  += (double) b;
    color /= 3;
    double max_range = letters.size() - 1;
    double range = letters.size() - 1;
    // 255   -> idx = max_range
    // color -> idx = ?
    int idx = (int) ( color * max_range / 255.0 );
    return letters[ range - idx ];
}

Color::~Color()
{
    //dtor
}
