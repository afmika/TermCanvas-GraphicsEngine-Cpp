#include <iostream>
#include <thread>
#include <chrono>
#include <cmath>

#include "TermCanvasInc.h"

const int width  = 30;
const int height = 24;
int daxis = 0;

bool updateRect(Graphics::Point2D& rect, int& dx, int& dy, int dim ) {
    bool update = false;
    if ( rect.x + dim >= width  || rect.x <= 0 ) {
        dx *= -1;
        update = true;
    }
    if ( rect.y + dim >= height || rect.y <= 0 ) {
        dy *= -1;
        update = true;
    }
    rect.x += dx;
    rect.y += dy;
    return update;
}

int func_def(int x) {
    return 5 * std::sin(x + daxis) + 10;
}

int main() {
    TermCanvas canvas(width, height);
    canvas.setColorPalette( Palette::STD_GRAY );
    Color cfill   {1, 20, 10};
    Color cstroke {3, 20, 10};

    canvas.setStrokeColor(cstroke);
    canvas.setFillColor  (cfill);

    Graphics::Point2D rect {10, 6};
    Graphics::Point2D arc   {width / 2, height / 2};
    int dx  = 1, dy  = 1;
    int dim = 7;
    int radius = 1;

    srand(time(NULL));

    while ( true ) {
        canvas.clear();

        if ( updateRect(rect, dx, dy, dim) ) {
            cstroke.r = rand() % 255;
            cstroke.g = rand() % 255;
            cstroke.b = rand() % 255;
        }

        canvas.plot2d(&func_def, {2, width});
        daxis++;

        canvas.setStrokeColor(cstroke);
        canvas.setFillColor  (cfill);

        canvas.strokeRect(rect.x, rect.y, dim, dim);
        canvas.strokeCircle(arc.x, arc.y, radius++);
        canvas.text(rect.x + 1, rect.y + 3, "afmika");
        canvas.text(4, 2, "Hello World!");

        radius %= std::max(width, height);

        canvas.draw();

        std::this_thread::sleep_for(std::chrono::milliseconds(1000 / 24));
    }
    return 0;
}
