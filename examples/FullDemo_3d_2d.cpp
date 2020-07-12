#include <iostream>
#include <thread>
#include <chrono>
#include <cmath>

#include "TermCanvasInc.h"

const int width  = 80;
const int height = 35;
int deltax = 1;

void animateFace(char axis, double angle, int scale, TermCanvas& canvas, Graphics::Figure::Shape& cube ) {
    Graphics::Figure::Shape to_draw = Graphics::Transf::scale(cube, scale);
    canvas.drawShape( to_draw );

    // 3d rotation
    cube = Graphics::Transf::rotate(cube, angle, axis);
}

int func_def (int x) {
    return 5 * std::sin(x + deltax);
}

int main() {
    TermCanvas canvas(width, height);
    canvas.setColorPalette( Palette::STD_GRAY );

    Color cfill   {50, 20, 10};
    Color cstroke {5, 5, 10};

    canvas.setStrokeColor(cstroke);
    canvas.setFillColor  (cfill);

    Graphics::Figure::Polygon3D bottom {
        {0, 0, 0},
        {0, 1, 0},
        {1, 1, 0},
        {1, 0, 0}
    };
    Graphics::Figure::Polygon3D top {
        {0, 0, 1},
        {0, 1, 1},
        {1, 1, 1},
        {1, 0, 1}
    };
    Graphics::Figure::Polygon3D face1 {
        {0, 0, 0},
        {0, 0, 1},
        {0, 1, 1},
        {0, 1, 0}
    };
    Graphics::Figure::Polygon3D face2 {
        {1, 0, 0},
        {1, 0, 1},
        {1, 1, 1},
        {1, 1, 0}
    };

    Graphics::Figure::Shape cube {
        bottom, top, face1, face2
    };

    int radius = 0;
    int dx = 1, dy = 1;
    Graphics::Point2D rect {2, 2};

    while ( true ) {
        canvas.clear();
        // cube
        canvas.restore();
        canvas.text(3, 2, "3D Test");
        canvas.moveTo(10, 20);
        // rotation test
        double angle = 0.1;
        int scale = 15;
        animateFace('x', angle, scale, canvas, cube);
        animateFace('z', angle, scale, canvas, cube);

        // circle
        canvas.restore();
        canvas.moveTo(3 * width  / 4, height / 2);
        canvas.text(-10, -height/ 2 + 2, "2D Test");

        canvas.strokeCircle(0, 0, radius++);
        radius %= 20;

        canvas.plot2d(&func_def, {-width / 4, width / 4});
        deltax++;

        // rect
        canvas.restore();
        canvas.fillRect(rect.x, rect.y, 5, 5);

        if ( rect.x + 5 >= width || rect.x <= 0 ) {
            dx *= -1;
        }
        if ( rect.y + 5 >= height || rect.y <= 0 ) {
            dy *= -1;
        }
        rect.x += dx; rect.y += dy;

        canvas.draw();
        std::this_thread::sleep_for(std::chrono::milliseconds(1000 / 24));
    }
}
