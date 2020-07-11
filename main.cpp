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
    return 5 * std::sin(x + daxis) + 10;
}

void animateFace(char axis, double angle, int scale, TermCanvas& canvas, Graphics::Figure::Polygon3D& face ) {
    Graphics::Point3D head = face[0];
    int sc = scale;
    for (size_t i = 1; i < face.size(); i++) {
        Graphics::Point3D p3d = face[i];
        canvas.line(head.scale(sc).x, head.scale(sc).y, p3d.scale( sc ).x, p3d.scale( sc ).y);
        head = p3d;
    }

    // 3d rotation
    // Graphics::Type::Matrix rot_mat = Graphics::Math::matRot3d(angle, axis);
    for (size_t i = 0; i < face.size(); i++) {
        // face[i] = Graphics::Transf::apply(face[i], rot_mat);
        face[i] = Graphics::Transf::rotate(face[i], angle, axis);
    }
}

int main() {
    TermCanvas canvas(width, height);
    canvas.setColorPalette( Palette::STD_GRAY );

// set to 1 to run 2d tests
// set to 0 to run 3d tests
#define NOT_3D 0

#if NOT_3D
    Color cfill   {1, 20, 10};
    Color cstroke {3, 20, 10};

    canvas.setStrokeColor(cstroke);
    canvas.setFillColor  (cfill);

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

        canvas.plot2d(&func_def, {2, width});
        daxis++;

        canvas.setStrokeColor(cstroke);
        canvas.setFillColor  (cfill);

        canvas.strokeRect(rect2.x, rect2.y, dim2, dim2);
        // canvas.fillRect(rect1.x, rect1.y, dim1, dim1);
        // canvas.strokeCircle(arc.x, arc.y, radius);
        canvas.text(rect2.x + 1, rect2.y + 3, "afmika");
        canvas.text(4, 2, "Hello World!");

        canvas.draw();

        std::this_thread::sleep_for(std::chrono::milliseconds(1000 / 24));
    }
#else
    Color cfill   {255, 20, 10};
    Color cstroke {5, 5, 10};

    canvas.setStrokeColor(cstroke);
    canvas.setFillColor  (cfill);

    Graphics::Figure::Polygon3D bottom {
        {0, 0, 0},
        {0, 1, 0},
        {1, 1, 0},
        {1, 0, 0},
        {0, 0, 0},
    };
    Graphics::Figure::Polygon3D top {
        {0, 0, 1},
        {0, 1, 1},
        {1, 1, 1},
        {1, 0, 1},
        {0, 0, 1},
    };
    Graphics::Figure::Polygon3D face1 {
        {0, 0, 0},
        {0, 0, 1},
        {0, 1, 1},
        {0, 1, 0},
        {0, 0, 0},
    };
    Graphics::Figure::Polygon3D face2 {
        {1, 0, 0},
        {1, 0, 1},
        {1, 1, 1},
        {1, 1, 0},
        {1, 0, 0},
    };

    while ( true ) {
        canvas.clear();
        canvas.restore();
        canvas.text(1, 1, "3D Hello world!");
        canvas.moveTo(width / 2 , height / 2 );

        // rotation test
        double angle = 0.1;
        int scale = 15;
        animateFace('x', angle, scale, canvas, bottom);
        animateFace('x', angle, scale, canvas, top);
        animateFace('x', angle, scale, canvas, face1);
        animateFace('x', angle, scale, canvas, face2);

        animateFace('z', angle, scale, canvas, bottom);
        animateFace('z', angle, scale, canvas, top);
        animateFace('z', angle, scale, canvas, face1);
        animateFace('z', angle, scale, canvas, face2);

        canvas.draw();

        std::this_thread::sleep_for(std::chrono::milliseconds(1000 / 60));
    }
#endif
    return 0;
}
