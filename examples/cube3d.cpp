#include <iostream>
#include <thread>
#include <chrono>
#include <cmath>

#include "TermCanvasInc.h"

const int width  = 30;
const int height = 24;

void animateFace(char axis, double angle, int scale, TermCanvas& canvas, Graphics::Figure::Shape& cube ) {
    Graphics::Figure::Shape to_draw = Graphics::Transf::scale(cube, scale);
    canvas.drawShape( to_draw );

    // 3d rotation
    cube = Graphics::Transf::rotate(cube, angle, axis);
}

int main() {
    TermCanvas canvas(width, height);
    canvas.setColorPalette( Palette::STD_GRAY );
	
    Color cfill   {255, 20, 10};
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

    while ( true ) {
        canvas.clear();
        canvas.restore();
        canvas.text(1, 1, "3D Hello world!");
        canvas.moveTo(width / 2 , height / 2 );

        // rotation test
        double angle = 0.1;
        int scale = 15;
        animateFace('x', angle, scale, canvas, cube);
        animateFace('z', angle, scale, canvas, cube);

        canvas.draw();

        std::this_thread::sleep_for(std::chrono::milliseconds(1000 / 60));
    }
}
