#ifndef __S21MATRIX_H__
#define __S21MATRIX_H__

#include <math.h>

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <limits>

class S21Matrix {
 public:
  S21Matrix();                                // default constructor
  S21Matrix(const int rows, const int cols);  // parameterized constructor
  S21Matrix(const int rows, const int cols, double min, double max);
  S21Matrix(const S21Matrix& other);  // copy constructor
  S21Matrix(S21Matrix&& other);       // move constructor with rvalue reference
  ~S21Matrix();                       // destructor

  // accessors and mutators
  int GetRows() const noexcept;
  int GetCols() const noexcept;
  void SetRows(const int rows);
  void SetCols(const int cols);

  // operators overloads
  S21Matrix operator+(const S21Matrix& other) const;
  S21Matrix operator-(const S21Matrix& other) const;
  S21Matrix operator*(const S21Matrix& other) const;
  S21Matrix operator*(const double num) const;
  friend S21Matrix operator*(const double num, const S21Matrix& other);
  bool operator==(const S21Matrix& other) const noexcept;
  S21Matrix& operator=(const S21Matrix& other);
  S21Matrix& operator+=(const S21Matrix& other);
  S21Matrix& operator-=(const S21Matrix& other);
  S21Matrix& operator*=(const S21Matrix& other);
  S21Matrix& operator*=(const double num);
  double& operator()(const int i, const int j);

  // public methods
  bool EqMatrix(const S21Matrix& other) const noexcept;
  void SumMatrix(const S21Matrix& other);
  void SubMatrix(const S21Matrix& other);
  void MulNumber(const double num) noexcept;
  void MulMatrix(const S21Matrix& other);
  S21Matrix Transpose() const;
  S21Matrix CalcComplements() const;
  double Determinant() const;
  S21Matrix InverseMatrix() const;

  void PrintMatrix();

 private:
  // attributes
  int rows_, cols_;  // rows and columns attributes
  double** matrix_;  // pointer to the memory where the matrix will be allocated

  S21Matrix CutElement(const int col, const int row) const;
  void CreateMatrix();
  void RemoveMatrix();
  double RandNum(double min, double max);
};

#endif
