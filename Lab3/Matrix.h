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
	void allocate_cells(int, int);
	void free_cells();
public:
	Matrix() : rows(0), cols(0), cells(nullptr) {}	// Конструктор по умолчанию
	Matrix(const Matrix&);							// Конструктор копирования
	explicit Matrix(int);
	Matrix(int, int);								// Конструктор нулевой матрицы
	Matrix(int, int, Cell*);						// Конструктор матрицы из списка
	~Matrix();										// Деструктор

	Matrix transpose();
	Matrix column(int number);
	Matrix row(int number);
	bool same_size_with(const Matrix&);
	int rows_count();
	int cols_count();
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
	allocate_cells(M.rows, M.cols);
	for (int i=0; i<rows; i++)
	for (int j=0; j<cols; j++)
		cells[i][j] = M.cells[i][j];
}

template <typename Cell>
Matrix<Cell>::Matrix(int Size)
{
	allocate_cells(Size, Size);
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
			cells[i][j] = 0;
}

template <typename Cell>
Matrix<Cell>::Matrix(int Rows, int Cols)
{
	allocate_cells(Rows, Cols);
	for (int i=0; i<Rows; i++)
		for (int j=0; j<Cols; j++)
			cells[i][j] = 0;
}

template <typename Cell>
Matrix<Cell>::Matrix(int Rows, int Cols, Cell* list)
{
	allocate_cells(Rows, Cols);
	for (int i = 0; i < Rows; i++) {
		for (int j = 0; j < Cols; j++) {
			cells[i][j] = list[ i * cols + j];
		}
	}
}

template <typename Cell>
Matrix<Cell>::~Matrix()
{
	free_cells();
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
bool Matrix<Cell>::same_size_with(const Matrix& M) {
	return rows == M.rows && cols == M.cols;
}

template <typename Cell>
int Matrix<Cell>::rows_count() {
	return rows;
}

template <typename Cell>
int Matrix<Cell>:: cols_count() {
	return cols;
}

template <typename Cell>
void Matrix<Cell>:: print() {
	for (int i = 0; i < rows_count(); i++) {
		for (int j = 0; j < cols_count(); j++) {
			cout << cells[i][j] << '\t';
		}
		cout << endl;
	}
	cout << "------------------" << endl;
}

template <typename Cell>
Matrix<Cell>& Matrix<Cell>::operator=(const Matrix& M)
{
	if ( !same_size_with(M) )
	{
		free_cells();
		allocate_cells(M.rows, M.cols);
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
	if ( same_size_with(M) )
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
	if (same_size_with(M))
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
void Matrix<Cell>::allocate_cells(int Rows, int Cols)
{
	cells = new Cell*[Rows];
	for (int i=0; i<Rows; i++)
		cells[i] = new Cell[Cols];
	rows = Rows;
	cols = Cols;
}

template <typename Cell>
void Matrix<Cell>::free_cells()
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
