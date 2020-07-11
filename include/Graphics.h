#ifndef GRAPHICS_H_INCLUDED
#define GRAPHICS_H_INCLUDED
#include <cmath>

namespace Graphics {
    typedef struct Point2D {
        double x;
        double y;
        Point2D(int x, int y) : x(x), y(y) {
            this->x = x;
            this->y = y;
        }
    } Point2D;

    typedef struct Point3D {
        double x;
        double y;
        double z;
        Point3D(int x, int y, int z) : x(x), y(y), z(z) {
            this->x = x;
            this->y = y;
            this->y = z;
        }
    } Point3D;

    typedef struct Matrix2D {

    } Matrix2D;

    typedef struct Matrix3D {
    } Matrix3D;

    // todo
    void transform(Point2D& point, Matrix2D m);
    void transform(Point3D& point, Matrix3D m);

    void rotate(Point2D& point, double teta) {
        double x = point.x, y = point.y;
        point.x = x * std::cos(teta) + y * std::sin(teta);
        point.y = x *-std::sin(teta) + y * std::cos(teta);
    }

    void rotate(Point3D point);
}



#endif // GRAPHICS_H_INCLUDED
