#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>

using namespace std;


template <typename Cell = double>
class Matrix
{
private:
	int rows, cols;
	Cell** cells;
	void AllocateCells(int, int);
	void FreeCells();
public:
	Matrix() : rows(0), cols(0), cells(nullptr) {}	// Конструктор по умолчанию
	Matrix(const Matrix&);							// Конструктор копирования
	Matrix(int);
	Matrix(int, int);								// Конструктор нулевой матрицы
	Matrix(int, int, Cell*);						// Конструктор матрицы из списка
	~Matrix();										// Деструктор

	Matrix transpose();
	Matrix column(int number);
	Matrix row(int number);
	bool sameSizeWith(const Matrix&);
	int rowsCount();
	int colsCount();
	void print();

	Cell &operator()(int i, int j) { return cells[i][j]; }

	Matrix& operator = (const Matrix&);		// Перегрузка оператора присваивания
	Matrix  operator + (const Matrix&);		// Сложение матриц
	Matrix  operator - (const Matrix&);		// Вычитание матриц
	Matrix  operator * (const Matrix&);		// Умножение матриц
	Matrix  operator * (const double);

	friend istream& operator >> <> (istream&, Matrix&);			// Перегрузка оператора >> для ввода матрицы
	friend ostream& operator << <> (ostream&, const Matrix&);	// Перегрузка оператора << для вывода матрицы
};


template <typename Cell>
Matrix<Cell>::Matrix(const Matrix<Cell>& M)
{
	AllocateCells(M.rows, M.cols);
	for (int i=0; i<rows; i++)
	for (int j=0; j<cols; j++)
		cells[i][j] = M.cells[i][j];
}

template <typename Cell>
Matrix<Cell>::Matrix(int Size)
{
	AllocateCells(Size, Size);
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
			cells[i][j] = 0;
}

template <typename Cell>
Matrix<Cell>::Matrix(int Rows, int Cols)
{
	AllocateCells(Rows, Cols);
	for (int i=0; i<Rows; i++)
		for (int j=0; j<Cols; j++)
			cells[i][j] = 0;
}

template <typename Cell>
Matrix<Cell>::Matrix(int Rows, int Cols, Cell* list)
{
	AllocateCells(Rows, Cols);
	for (int i = 0; i < Rows; i++) {
		for (int j = 0; j < Cols; j++) {
			cells[i][j] = list[ i * cols + j];
		}
	}
}

template <typename Cell>
Matrix<Cell>::~Matrix()
{
	FreeCells();
}

template <typename Cell>
Matrix<Cell> Matrix<Cell>::transpose() {
	Matrix<> res(cols, rows);
	for (int i = 0; i < res.rows; i++) {
		for (int j = 0; j < res.cols; j++) {
			res.cells[i][j] = cells[j][i];
		}
	}
	return res;
}

template <typename Cell>
Matrix<Cell> Matrix<Cell>::row(int number) {
	return Matrix<>(1, cols, cells[number]);
}

template <typename Cell>
Matrix<Cell> Matrix<Cell>::column(int number) {
	Matrix<> res(rows, 1);
	for (int i = 0; i < rows; i++) {
		res.cells[i][0] = cells[i][number];
	}
	return res;
}

template <typename Cell>
bool Matrix<Cell>::sameSizeWith(const Matrix& M) {
	return rows == M.rows && cols == M.cols;
}

template <typename Cell>
int Matrix<Cell>::rowsCount() {
	return rows;
}

template <typename Cell>
int Matrix<Cell>:: colsCount() {
	return cols;
}

template <typename Cell>
void Matrix<Cell>:: print() {
	for (int i = 0; i < rowsCount(); i++) {
		for (int j = 0; j < colsCount(); j++) {
			cout << cells[i][j] << '\t';
		}
		cout << endl;
	}
	cout << "------------------" << endl;
}

template <typename Cell>
Matrix<Cell>& Matrix<Cell>::operator=(const Matrix& M)
{
	if ( !sameSizeWith(M) )
	{
		FreeCells();
		AllocateCells(M.rows, M.cols);
	}
	for (int i=0; i<rows; i++)
	for (int j=0; j<cols; j++)
		cells[i][j] = M.cells[i][j];
	return *this;
}

template <typename Cell>
Matrix<Cell> Matrix<Cell>::operator+(const Matrix& M)
{
	Matrix<Cell> res(*this);
	if ( sameSizeWith(M) )
	{
		for (int i=0; i<rows; i++)
		for (int j=0; j<cols; j++)
			res.cells[i][j] += M.cells[i][j];
	}
	return res;
}

template <typename Cell>
Matrix<Cell> Matrix<Cell>::operator-(const Matrix& M)
{
	Matrix<Cell> res(*this);
	if (sameSizeWith(M))
	{
		for (int i = 0; i < rows; i++)
			for (int j = 0; j < cols; j++)
				res.cells[i][j] -= M.cells[i][j];
	}
	return res;
}

template <typename Cell>
Matrix<Cell> Matrix<Cell>::operator*(const Matrix& M)
{
	Matrix<Cell> res(rows, M.cols);
	if (cols == M.rows) {
		for (int i = 0; i < res.rows; i++) {
			for (int j = 0; j < res.cols; j++) {
				Cell sum = 0;
				for (int k = 0; k < cols; k++) {
					sum += cells[i][k] * M.cells[k][j];
				}
				res.cells[i][j] = sum;
			}
		}
	}
	return res;
}

template <typename Cell>
Matrix<Cell> Matrix<Cell>::operator*(const double scalar)
{
	Matrix<Cell> res(rows, cols);
	for (int i = 0; i < res.rows; i++) {
		for (int j = 0; j < res.cols; j++) {
			res.cells[i][j] = cells[i][j] * scalar;
		}
	}
	return res;
}

template <typename Cell>
void Matrix<Cell>::AllocateCells(int Rows, int Cols)
{
	cells = new Cell*[Rows];
	for (int i=0; i<Rows; i++)
		cells[i] = new Cell[Cols];
	rows = Rows;
	cols = Cols;
}

template <typename Cell>
void Matrix<Cell>::FreeCells()
{
	for (int i=0; i<rows; i++)
		delete cells[i];
	delete cells;
	rows = 0;
	cols = 0;
}

template <typename Cell>
istream& operator >> (istream& fi, Matrix<Cell>& M)
{
	for (int i=0; i<M.rows; i++)
	for (int j=0; j<M.cols; j++)
		fi >> M.cells[i][j];
	return fi;
}

template <typename Cell>
ostream& operator << (ostream& fo, const Matrix<Cell>& M)
{
	for (int i=0; i<M.rows; i++)
	{
		for (int j=0; j<M.cols; j++)
			fo << M.cells[i][j] << " \t";
		fo << endl;
	}
	return fo;
}

#endif MATRIX_H
