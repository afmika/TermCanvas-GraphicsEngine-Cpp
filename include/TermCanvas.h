#ifndef TERMCANVAS_H
#define TERMCANVAS_H

#include <iostream>
#include <cstdint>
#include <vector>
#include <stdio.h>

#ifdef _WIN32
    #include <windows.h>
#endif // _WIN32

#include "Color.h"

class TermCanvas {
public:
    // color palette
    std::string DEFAULT_COLOR_PALETTE = "* ";
    uint32_t col;
    uint32_t row;
    Color default_dead_cell {0 , 0, 0};
    Color default_alive_cell{255, 255, 255};


    std::vector<std::vector<char>> *screen;

    TermCanvas(uint32_t col_value, uint32_t row_value);
    TermCanvas();
    virtual ~TermCanvas();


    void draw() const;
    void clear();

    bool isInside(int x, int y);
    void set(int x, int y, Color value );
    void set(int x, int y, char charact );
    void set(int x, int y);
    void setColorPalette(std::string letters);

    // drawing
    void line(int xi, int yi, int xf, int yf, Color value);
    void line(int xi, int yi, int xf, int yf);

    void fillRect(int x, int y, int width, int height);
    void strokeRect(int x, int y, int width, int height);

    void strokeCircle(int x, int y, int radius);
    void fillCircle(int x, int y, int radius);

    void plot2d(int (*f)(int), std::pair<int, int> x_range);

    void text(int x, int y, std::string text_value);

    void setFillColor(Color color);
    void setStrokeColor(Color color);
    Color getFillColor() const;
    Color getStrokeColor() const;

private:
    Color fill_color{255, 255, 255};
    Color stroke_color{255, 255, 255};
    void init (uint32_t col_value, uint32_t row_value);
};


#endif // TERMCANVAS_H
