#pragma once

#include <vector>
#include <iostream>


namespace task {

    const double EPS = 1e-6;


    class OutOfBoundsException : public std::exception {
    };

    class SizeMismatchException : public std::exception {
    };


    class Matrix {

    private:
        double **matrix_;
        size_t col_num_;
        size_t row_num_;

    public:

        Matrix();

        Matrix(size_t rows, size_t cols);

        Matrix(const Matrix &copy);

        Matrix &operator=(const Matrix &a);

        double &get(size_t row, size_t col);

        const double &get(size_t row, size_t col) const;

        void set(size_t row, size_t col, const double &value);

        void resize(size_t new_rows, size_t new_cols);

        double *&operator[](size_t row);

        const double *&operator[](size_t row) const;

        Matrix &operator+=(const Matrix &a);

        Matrix &operator-=(const Matrix &a);

        Matrix &operator*=(const Matrix &a);

        Matrix &operator*=(const double &number);

        Matrix operator+(const Matrix &a) const;

        Matrix operator-(const Matrix &a) const;

        Matrix operator*(const Matrix &a) const;

        Matrix operator*(const double &a) const;

        Matrix operator-() const;

        Matrix operator+() const;

        Matrix getMinor(const size_t &row, const size_t &col) const;

        double det() const;

        void transpose();

        Matrix transposed() const;

        double trace() const;

        std::vector<double> getRow(size_t row);

        std::vector<double> getColumn(size_t column);

        bool operator==(const Matrix &a) const;

        bool operator!=(const Matrix &a) const;

        size_t getRowNum() const {
            return row_num_;
        }

        size_t getColNum() const {
            return col_num_;
        }

    };


    Matrix operator*(const double &a, const Matrix &b);

    std::ostream &operator<<(std::ostream &output, const Matrix &matrix);

    std::istream &operator>>(std::istream &input, Matrix &matrix);


}  // namespace task
