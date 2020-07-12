#include <iostream>
#include <thread>
#include <chrono>
#include <cmath>

#include "TermCanvasInc.h"

const int width  = 30;
const int height = 24;

int func_def(int x) {
    return 5 * std::sin(x);
}

int main() {
    TermCanvas canvas(width, height);
    canvas.setColorPalette( Palette::STD_GRAY );

    while ( true ) {
        canvas.restore();
        canvas.clear();
        canvas.text(1, 1, "y = 5*sin(x)");
        canvas.moveTo(width / 2, height / 2);
        canvas.plot2d(&func_def, {-width/2, width/2});
        canvas.draw();
        std::this_thread::sleep_for(std::chrono::milliseconds(1000 / 24));
    }
	return 0;
}