#include <cmath>
#include "matrix.h"

using namespace task;


Matrix::Matrix() {
    col_num_ = 1;
    row_num_ = 1;
    matrix_ = new double*[1];
    matrix_[0] = new double[1]{1.0};
}

Matrix::Matrix(size_t rows, size_t cols) {
    row_num_ = rows;
    col_num_ = cols;

    matrix_ = new double*[rows];
    for(size_t i = 0; i < rows; ++i) {
        matrix_[i] = new double[col_num_]{0.0};
    }

    for(size_t i = 0; i < std::min(rows, cols); ++i) {
        matrix_[i][i] = 1.0;
    }
}

Matrix::Matrix(const Matrix& copy) {
    this->row_num_ = copy.row_num_;
    this->col_num_ = copy.col_num_;
    this->matrix_= new double*[row_num_];
    for(size_t i = 0; i < row_num_; ++i) {
        this->matrix_[i] = new double[col_num_];
        for(size_t j = 0; j < col_num_; ++j) {
            this->matrix_[i][j] = copy[i][j];
        }
    }
}

Matrix& Matrix::operator=(const Matrix& a) {
    if(&a == this) {
        return *this;
    }

    delete[] this->matrix_;
    this->row_num_ = a.row_num_;
    this->col_num_ = a.col_num_;
    this->matrix_ = new double*[row_num_];
    for(size_t i = 0; i < row_num_; ++i) {
        this->matrix_[i] = new double[col_num_];
        for(size_t j = 0; j < col_num_; ++j) {
            this->matrix_[i][j] = a[i][j];
        }
    }
    return *this;
}

double& Matrix::get(size_t row, size_t col) {
    if(row >= row_num_ || col >= col_num_) {
        throw OutOfBoundsException();
    }
    return matrix_[row][col];
}

const double& Matrix::get(size_t row, size_t col) const {
    if(row >= row_num_ || col >= col_num_) {
        throw OutOfBoundsException();
    }
    return matrix_[row][col];
}

void Matrix::set(size_t row, size_t col, const double& value) {
    if(row >= row_num_ || col >= col_num_) {
        throw OutOfBoundsException();
    }
    matrix_[row][col] = value;
}

void Matrix::resize(size_t new_rows, size_t new_cols) {
    double** newMatrix = new double*[new_rows];
    for(size_t i = 0; i < new_rows; ++i) {
        newMatrix[i] = new double[new_cols]{0};
    }
    for(size_t i = 0; i < std::min(new_rows, row_num_); ++i) {
        for(size_t j = 0; j < std::min(new_cols, col_num_); ++j) {
            newMatrix[i][j] = matrix_[i][j];
        }
    }

    delete[] matrix_;
    matrix_ = newMatrix;
    row_num_ = new_rows;
    col_num_ = new_cols;
}

double*& Matrix::operator[](size_t row) {
    if(row >= row_num_) {
        throw OutOfBoundsException();
    }
    return matrix_[row];
}

const double*& Matrix::operator[](size_t row) const {
    if(row >= row_num_) {
        throw OutOfBoundsException();
    }
    return (const double *&) matrix_[row];
}

Matrix& Matrix::operator+=(const Matrix& a) {
    if(row_num_ != a.row_num_ || col_num_ != a.col_num_) {
        throw SizeMismatchException();
    }
    for(size_t i = 0; i < row_num_; ++i) {
        for(size_t j = 0; j < col_num_; ++j) {
            matrix_[i][j] += a.matrix_[i][j];
        }
    }

    return *this;
}

Matrix& Matrix::operator-=(const Matrix& a) {
    if(row_num_ != a.row_num_ || col_num_ != a.col_num_) {
        throw SizeMismatchException();
    }
    for(size_t i = 0; i < row_num_; ++i) {
        for(size_t j = 0; j < col_num_; ++j) {
            matrix_[i][j] -= a.matrix_[i][j];
        }
    }

    return *this;
}

Matrix& Matrix::operator*=(const Matrix& a) {
    Matrix newMatrix = *this * a;
    *this = newMatrix;
    return *this;
}

Matrix& Matrix::operator*=(const double& number) {
    for(size_t i = 0; i < row_num_; ++i) {
        for(size_t j = 0; j < col_num_; ++j) {
            matrix_[i][j] *= number;
        }
    }

    return *this;
}

Matrix Matrix::operator+(const Matrix& a) const {
    if(row_num_ != a.row_num_ || col_num_ != a.col_num_) {
        throw SizeMismatchException();
    }

    Matrix* newMatrix = new Matrix(*this);
    *newMatrix += a;

    return *newMatrix;
}

Matrix Matrix::operator-(const Matrix& a) const {
    if(row_num_ != a.row_num_ || col_num_ != a.col_num_) {
        throw SizeMismatchException();
    }

    Matrix* newMatrix = new Matrix(*this);
    *newMatrix -= a;

    return *newMatrix;
}

Matrix Matrix::operator*(const Matrix& a) const {
    if(col_num_ != a.row_num_) {
        throw SizeMismatchException();
    }

    Matrix* newMatrix = new Matrix(row_num_, a.col_num_);
    for(size_t i = 0; i < row_num_; ++i) {
        for(size_t j = 0; j < a.col_num_; ++j) {
            for(size_t r = 0; r < col_num_; ++r) {
                newMatrix->matrix_[i][j] += matrix_[i][r] * a.matrix_[r][j];
            }
        }
    }

    return *newMatrix;
}

Matrix Matrix::operator*(const double& a) const {
    Matrix* newMatrix = new Matrix(*this);
    *newMatrix *= a;
    return *newMatrix;
}

Matrix Matrix::operator-() const {
    Matrix* newMatrix = new Matrix(*this);
    for(size_t i = 0; i < row_num_; ++i) {
        for(size_t j = 0; j < col_num_; ++j) {
            *newMatrix[i][j] *= -1.0;
        }
    }
    return *newMatrix;
}

Matrix Matrix::operator+() const {
    Matrix* newMatrix = new Matrix(*this);
    return *newMatrix;
}

double Matrix::det() const {
    if(col_num_ != row_num_|| row_num_ == 0) {
        throw SizeMismatchException();
    }
    if(row_num_ == 1) {
        return matrix_[0][0];
    }

    double det = 10000.0;


    return det;
}

void Matrix::transpose() {
    Matrix newMatrix = Matrix(col_num_, row_num_);
    for(size_t i = 0; i < row_num_; ++i) {
        for(size_t j = 0; j < col_num_; ++j) {
            newMatrix.matrix_[j][i] = matrix_[i][j];
        }
    }
    *this = newMatrix;
}

Matrix Matrix::transposed() const {
    Matrix* newMatrix = new Matrix(col_num_, row_num_);
    for(size_t i = 0; i < row_num_; ++i) {
        for(size_t j = 0; j < col_num_; ++j) {
            newMatrix->matrix_[j][i] = matrix_[i][j];
        }
    }
    return *newMatrix;
}

double Matrix::trace() const {
    if(row_num_ != col_num_) {
        throw SizeMismatchException();
    }
    double tr = 0.0;
    for(size_t i = 0; i < std::min(row_num_, col_num_); ++i) {
        tr += matrix_[i][i];
    }
    return tr;
}

std::vector<double> Matrix::getRow(size_t row) {
    if(row >= row_num_) {
        throw OutOfBoundsException();
    }
    double* mRow = (*this)[row];
    std::vector<double> v;
    for(size_t i = 0; i < col_num_; ++i) {
        v.push_back(mRow[i]);
    }
    return v;
}

std::vector<double> Matrix::getColumn(size_t column) {
    if(column >= col_num_) {
        throw OutOfBoundsException();
    }
    std::vector<double> v;
    for(size_t i = 0; i < row_num_; ++i) {
        v.push_back(matrix_[i][column]);
    }
    return v;
}

bool Matrix::operator==(const Matrix& a) const {
    if(col_num_ != a.col_num_ || row_num_ != a.row_num_) {
        return false;
    }
    for(size_t i = 0; i < row_num_; ++i) {
        for(size_t j = 0; j < col_num_; ++j) {
            if(fabs((*this)[i][j] - a[i][j]) > EPS) {
                return false;
            }
        }
    }
    return true;
}

bool Matrix::operator!=(const Matrix& a) const {
    return !((*this) == a);
}

Matrix task::operator*(const double& a, const Matrix& b) {
    Matrix* newMatrix = new Matrix(b);
    *newMatrix *= a;
    return *newMatrix;
}

std::ostream& task::operator<<(std::ostream& output, const Matrix& matrix) {
    for(size_t i = 0; i < matrix.getRowNum(); ++i) {
        for(size_t j = 0; j < matrix.getColNum(); ++j) {
            output << matrix[i][j] << " ";
        }
        output << std::endl;
    }
    return output;
}

std::istream& task::operator>>(std::istream& input, Matrix& matrix) {
    size_t rowNum, colNum;
    input >> rowNum >> colNum;
    matrix.resize(rowNum, colNum);
    for(size_t i = 0; i < rowNum; ++i) {
        for(size_t j = 0; j < colNum; ++j) {
            input >> matrix[i][j];
        }
    }
    return input;
}