#include "Matrices.h"
#include <cmath>

using namespace std;

namespace Matrices {

  
    Matrix::Matrix(int _rows, int _cols) {
        rows = _rows;
        cols = _cols;
        a.assign(rows, vector<double>(cols, 0.0));
    }

    Matrix operator+(const Matrix& a, const Matrix& b) {
        Matrix c(a.getRows(), a.getCols());
        for (int i = 0; i < a.getRows(); ++i) {
            for (int j = 0; j < a.getCols(); ++j) {
                c(i, j) = a(i, j) + b(i, j);
            }
        }
        return c;
    }

    Matrix operator*(const Matrix& a, const Matrix& b) {
        Matrix c(a.getRows(), b.getCols());
        for (int i = 0; i < a.getRows(); ++i) {
            for (int j = 0; j < b.getCols(); ++j) {
                double sum = 0;
                for (int k = 0; k < a.getCols(); ++k) {
                    sum += a(i, k) * b(k, j);
                }
                c(i, j) = sum;
            }
        }
        return c;
    }





    RotationMatrix::RotationMatrix(double theta) : Matrix(2, 2) {
        (*this)(0, 0) = cos(theta);
        (*this)(0, 1) = -sin(theta);
        (*this)(1, 0) = sin(theta);

        (*this)(1, 1) = cos(theta);
    }

    ScalingMatrix::ScalingMatrix(double scale) : Matrix(2, 2) {
        (*this)(0, 0) = scale;
        (*this)(0, 1) = 0.0;
        (*this)(1, 0) = 0.0;
        (*this)(1, 1) = scale;
    }

    TranslationMatrix::TranslationMatrix(double xShift, double yShift, int nCols) : Matrix(2, nCols) {
        for (int j = 0; j < nCols; ++j) {
            (*this)(0, j) = xShift;
            (*this)(1, j) = yShift;
        }
    }
}