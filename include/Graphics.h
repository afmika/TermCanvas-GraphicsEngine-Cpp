#ifndef GRAPHICS_H_INCLUDED
#define GRAPHICS_H_INCLUDED
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
        Type::VecInt ConvertToInt(Type::Vec v) {
            Type::VecInt result;
            for (size_t i = 0; i < v.size(); i++)
                result.push_back( v[i] );
            return result;
        }
        Type::MatrixInt ConvertToInt(Type::Matrix v) {
            Type::MatrixInt result(v.size());
            for (size_t i = 0; i < v.size(); i++) {
                result[i] = *(new Type::VecInt(v[i].size()));
                for (size_t j = 0; j < v[i].size(); j++) {
                    result[i][j] = (int) v[i][j];
                }
            }
            return result;
        }
        std::string StringifyMatrix(Type::Matrix a) {
            std::string result = "";
            for (size_t i = 0; i < a.size(); i++) {
                result += "[ ";
                for (size_t j = 0; j < a[i].size(); j++) {
                    result += std::to_string(a[i][j]) + " ";
                }
                result += "]\n";
            }
            return result;
        }
        void displayMatrix(Type::Matrix a) {
            printf("%s", StringifyMatrix(a).c_str());
        }

        // creation
        Type::Matrix createMatrix(size_t row, size_t col) {
            Type::Matrix m(row);
            for (size_t r = 0; r < row; r++) {
                m[r] = *(new Type::Vec(col, 0));
            }
            return m;
        }
        Type::Matrix squareMatrix(size_t dim) {
            return createMatrix( dim, dim );
        }
        Type::Matrix identityMatrix(size_t dim) {
            Type::Matrix idm = squareMatrix(dim);
            for (size_t i = 0; i < dim; i++)
                idm[i][i] = 1;
            return idm;
        }
        Type::Matrix matRot2d(double angle) {
            using namespace std;
            double t = angle;
            return Type::Matrix {
                { cos(t), sin(t) },
                {-sin(t), cos(t) }
            };
        }
        Type::Matrix matRot3d(double angle, char axis) {
            using namespace std;
            double t = angle;
            Type::Matrix x_axis {
              {1, 0, 0},
              {0, cos(t), -sin(t)},
              {0, sin(t), cos(t)}
            };

            Type::Matrix y_axis {
              {cos(t),0, -sin(t)},
              {0, 1, 0},
              {sin(t),0, cos(t)}
            };

            Type::Matrix z_axis {
              {cos(t), -sin(t), 0},
              {sin(t), cos(t), 0},
              {0, 0, 1}
            };

            std::map<char, Type::Matrix> m {
                {'x', x_axis},
                {'y', y_axis},
                {'z', z_axis},
            };
            return m[axis];
        }

        // transpose
        Type::Matrix transpose(Type::Vec vect) {
            Type::Matrix result = createMatrix(vect.size(), 1);
            for ( size_t i = 0; i < vect.size(); i++)
                result[i][0] = vect[i];
            return result;
        }
        Type::Matrix transpose(Type::VecInt vect) {
            Type::Matrix result = createMatrix(vect.size(), 1);
            for ( size_t i = 0; i < vect.size(); i++)
                result[i][0] = vect[i];
            return result;
        }

        // operations
        Type::Matrix mult(Type::Matrix a, Type::Matrix b) {
            size_t nrow_a = a.size();
            size_t nrow_b = b.size();
            size_t ncol_a = a[0].size();
            size_t ncol_b = b[0].size();
            if ( ncol_a != nrow_b ) {
                throw Error::MATRIX_MULT_ERROR;
            }

            Type::Matrix out( nrow_a );

            for (size_t i = 0; i < nrow_a; i++) {
                out[i] = *( new Type::Vec(ncol_b) );
                for (size_t j = 0; j < ncol_b; j++) {
                    double s = 0;
                    for (size_t k = 0; k < nrow_b; k++) {
                        s += a[i][k] * b[k][j];
                    }
                    out[i][j] = s;
                }
            }
            return out;
        }
        Type::Vec addVec(Type::Vec a, Type::Vec b) {
            Type::Vec res;
            for (size_t i = 0; i < a.size(); i++) {
                res.push_back( a[i] + b[i] );
            }
            return res;
        }
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
    }


    namespace Transf {

        Point2D apply(Point2D point, Type::Matrix m) {
            Type::Matrix pt = point.toMatrix();
            Type::Matrix result = Math::mult(m, pt);
            Point2D out{result[0][0], result[1][0]};
            return out;
        }

        Point3D apply(Point3D point, Type::Matrix m) {
            Type::Matrix pt = point.toMatrix();
            Type::Matrix result = Math::mult(m, pt);
            Point3D out{result[0][0], result[1][0], result[2][0]};
            return out;
        }

        Point2D rotate(Point2D point, double angle) {
            Graphics::Type::Matrix rot_mat = Graphics::Math::matRot2d(angle);
            return Graphics::Transf::apply(point, rot_mat);
        }
        Point3D rotate(Point3D point, double angle, char axis) {
            Graphics::Type::Matrix rot_mat = Graphics::Math::matRot3d(angle, axis);
            return Graphics::Transf::apply(point, rot_mat);
        }
    };

}



#endif // GRAPHICS_H_INCLUDED
