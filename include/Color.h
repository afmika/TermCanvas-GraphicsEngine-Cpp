#ifndef COLOR_H
#define COLOR_H

#include <iostream>
#include <cstdint>

class Color {
public:
    uint8_t r;
    uint8_t g;
    uint8_t b;
    Color(uint8_t r, uint8_t g, uint8_t b);
    Color();
    virtual ~Color();
    uint32_t toUint32 ();
    char toChar (std::string letters);
};
#endif // COLOR_H
