#ifndef GRAPHICS_H
#define GRAPHICS_H


#include <cmath>
#include <iostream>
#include <vector>
#include <map>

namespace Graphics {

    namespace Type {
        typedef std::vector<double> Vec;
        typedef std::vector<int> VecInt;
        typedef std::vector<Vec> Matrix;
        typedef std::vector<VecInt> MatrixInt;
    };

    enum Error {
        MATRIX_MULT_ERROR
    };

    namespace Math {
        // tools
        Type::VecInt ConvertToInt(Type::Vec v);
        Type::MatrixInt ConvertToInt(Type::Matrix v);
        std::string StringifyMatrix(Type::Matrix a);
        void displayMatrix(Type::Matrix a);

        // creation
        Type::Matrix createMatrix(size_t row, size_t col);
        Type::Matrix squareMatrix(size_t dim);
        Type::Matrix identityMatrix(size_t dim);
        Type::Matrix matRot2d(double angle);
        Type::Matrix matRot3d(double angle, char axis);

        // transpose
        Type::Matrix transpose(Type::Vec vect);
        Type::Matrix transpose(Type::VecInt vect);

        // operations
        Type::Matrix mult(Type::Matrix a, Type::Matrix b);
        Type::Vec addVec(Type::Vec a, Type::Vec b);
    };


    typedef struct Point2D {
        double x;
        double y;
        Point2D(int x, int y) : x(x), y(y) {
            this->x = (double) x;
            this->y = (double) y;
        }
        Point2D(double x, double y) : x(x), y(y) {
            this->x = x;
            this->y = y;
        }
        Type::Matrix toMatrix() {
            return Math::transpose(Type::Vec{x, y});
        }
        Point2D scale (double value) {
            return Point2D{value * x, value * y};
        }
    } Point2D;

    typedef struct Point3D {
        double x;
        double y;
        double z;
        Point3D(int x, int y, int z) : x(x), y(y), z(z) {
            this->x = (double) x;
            this->y = (double) y;
            this->z = (double) z;
        }
        Point3D(double x, double y, double z) : x(x), y(y), z(z) {
            this->x = x;
            this->y = y;
            this->z = z;
        }
        Type::Matrix toMatrix() {
            return Math::transpose(Type::Vec{x, y, z});
        }
        Point3D scale (double value) {
            return Point3D{value * x, value * y, value * z};
        }
    } Point3D;

    namespace Figure {
        typedef std::vector<Point2D> Polygon2D;
        typedef std::vector<Point3D> Polygon3D;
        typedef std::vector<Polygon3D> Shape;
    };

    namespace Transf {
        Point2D apply(Point2D point, Type::Matrix m);
        Point3D apply(Point3D point, Type::Matrix m);
        Figure::Polygon2D apply(Figure::Polygon2D polygon, Type::Matrix m);
        Figure::Polygon3D apply(Figure::Polygon3D polygon, Type::Matrix m);
        Figure::Shape apply(Figure::Shape shape, Type::Matrix m);

        Point2D rotate(Point2D point, double angle);
        Point3D rotate(Point3D point, double angle, char axis);
        Figure::Polygon2D rotate(Figure::Polygon2D polygon, double angle, char axis);
        Figure::Polygon3D rotate(Figure::Polygon3D polygon, double angle, char axis);
        Figure::Shape rotate(Figure::Shape shape, double angle, char axis);

        Figure::Polygon3D scale(Figure::Polygon3D polygon, double scale);
        Figure::Polygon2D scale(Figure::Polygon2D polygon, double scale);
        Figure::Shape scale(Figure::Shape shape, double scale);
    };
};

#endif // GRAPHICS_H
