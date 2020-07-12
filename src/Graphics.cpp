#include "Graphics.h"

namespace Graphics {
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

    namespace Transf {
        // apply
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
        Figure::Polygon2D apply(Figure::Polygon2D polygon, Type::Matrix m) {
            Figure::Polygon2D result;
            for (auto point : polygon) {
                result.push_back( Transf::apply(point, m) );
            }
            return result;
        }
        Figure::Polygon3D apply(Figure::Polygon3D polygon, Type::Matrix m) {
            Figure::Polygon3D result;
            for (auto point : polygon) {
                result.push_back( Transf::apply(point, m) );
            }
            return result;
        }
        Figure::Shape apply(Figure::Shape shape, Type::Matrix m) {
            Figure::Shape result;
            for (auto polygon : shape) {
                result.push_back( Transf::apply(polygon, m) );
            }
            return result;
        }

        // rotate
        Point2D rotate(Point2D point, double angle) {
            Graphics::Type::Matrix rot_mat = Graphics::Math::matRot2d(angle);
            return Graphics::Transf::apply(point, rot_mat);
        }
        Point3D rotate(Point3D point, double angle, char axis) {
            Graphics::Type::Matrix rot_mat = Graphics::Math::matRot3d(angle, axis);
            return Graphics::Transf::apply(point, rot_mat);
        }
        Figure::Polygon2D rotate(Figure::Polygon2D polygon, double angle, char axis) {
            Figure::Polygon2D result;
            for (auto point : polygon) {
                result.push_back( Transf::rotate(point, angle) );
            }
            return result;
        }
        Figure::Polygon3D rotate(Figure::Polygon3D polygon, double angle, char axis) {
            Figure::Polygon3D result;
            for (auto point : polygon) {
                result.push_back( Transf::rotate(point, angle, axis) );
            }
            return result;
        }
        Figure::Shape rotate(Figure::Shape shape, double angle, char axis) {
            Figure::Shape result;
            for (auto polygon : shape) {
                result.push_back( Transf::rotate(polygon, angle, axis) );
            }
            return result;
        }

        // scale
        Figure::Polygon3D scale(Figure::Polygon3D polygon, double scale) {
            Figure::Polygon3D result;
            for (auto point : polygon) {
                result.push_back( point.scale(scale) );
            }
            return result;
        }
        Figure::Polygon2D scale(Figure::Polygon2D polygon, double scale) {
            Figure::Polygon2D result;
            for (auto point : polygon) {
                result.push_back( point.scale(scale) );
            }
            return result;
        }
        Figure::Shape scale(Figure::Shape shape, double scale) {
            Figure::Shape result;
            for (auto polygon : shape) {
                result.push_back( Transf::scale(polygon, scale) );
            }
            return result;
        }
    };

};
