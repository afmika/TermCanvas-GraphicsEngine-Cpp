#include <iostream>
#include <thread>
#include <chrono>
#include <cmath>

#include <TermCanvas.h>
#include <Graphics.h>
#include <Palette.h>

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

bool updateCircle(Graphics::Point2D& arc, int& dx, int& dy, int radius ) {
    bool update = false;
    if ( arc.x + radius >= width  || arc.x - radius <= 0 ) {
        dx *= -1;
        update = true;
    }
    if ( arc.y + radius >= height || arc.y - radius <= 0 ) {
        dy *= -1;
        update = true;
    }
    arc.x += dx;
    arc.y += dy;
    return update;
}

int func_def(int x) {
    return 20 / x + 10;
}

int main() {

    TermCanvas canvas(width, height);
    canvas.setColorPalette( Palette::STD_GRAY );

    Graphics::Point2D p1 {0, 0};
    Graphics::Point2D p2 {20, 20};

    Graphics::Point2D rect1 {10, 6};
    Graphics::Point2D rect2 {15, 3};
    Graphics::Point2D arc   {4, 4};
    int dx1  = 1, dy1  = 1;
    int dx2  = 1, dy2  = -1;
    int dx3  = 1, dy3  = -1;
    int dim1 = 4, dim2 = 8;
    int radius = 3;
    Color cfill   {1, 20, 10};
    Color cstroke {3, 20, 10};
    srand(time(NULL));

    while ( true ) {
        canvas.clear();

        if ( updateRect(rect1, dx1, dy1, dim1) ) {
            cfill.r = rand() % 255;
            cfill.g = rand() % 255;
            cfill.b = rand() % 255;
        }

        updateRect(rect2, dx2, dy2, dim2);
        updateCircle(arc, dx3, dy3, radius);

        canvas.fillRect(rect1.x, rect1.y, dim1, dim1);
        canvas.strokeRect(rect2.x, rect2.y, dim2, dim2);

        canvas.plot2d(&func_def, {2, width});
        daxis++;

        canvas.setStrokeColor(cstroke);
        canvas.setFillColor  (cfill);
        canvas.text(4, 2, "Hello World!");
        canvas.text(rect2.x + 1, rect2.y + 3, "afmika");
        canvas.strokeCircle(arc.x, arc.y, radius);

        // Graphics::rotate(p2, 3.14 / 12);
        canvas.draw();

        std::this_thread::sleep_for(std::chrono::milliseconds(1000 / 16));
    }
    return 0;
}
