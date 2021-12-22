#pragma once
#include <vector>
#include <fstream>
#include <string>
#include "Matrix.h"

bool Khaletsky(Matrix<> A, Matrix<>& B, Matrix<>& C)
{
	int size = A.rowsCount();
	for (int j = 0; j < size; j++) {
		for (int i = j; i < size; i++) {
			double sum = 0;
			for (int k = 0; k <= j - 1; k++) {
				sum += B(i, k) * C(k, j);
			}
			B(i, j) = A(i, j) - sum;
		}

		for (int i = j + 1; i < size; i++) {
			double sum = 0;
			for (int k = 0; k <= j - 1; k++) {
				sum += B(j, k) * C(k, i);
			}
			if (B(j, j) == 0) {
				// Разложение невозможно
				return false;
			}
			C(j, i) = (A(j, i) - sum) / B(j, j);
		}
	}
	// Решено успешно
	return true;
}
double vectorLength(Matrix<> v)
{
	const int SIZE = v.colsCount();
	double result = 0;

	for (int i = 0; i < SIZE; i++){
		result += v(0, i) * v(0, i);
	}
	return sqrt(result);

}
void normalize(Matrix<>& v) {
	const int SIZE = v.colsCount();
	double len = vectorLength(v);
	// Нормализуем вектор
	for (int i = 0; i < SIZE; i++) {
		v(0, i) /= len;
	}
};
double cosBetweenVectors(Matrix<> v1, Matrix<> v2)
{
	if (!v1.sameSizeWith(v2))
		return 99;

	double result = 0;
	const int SIZE = v1.colsCount();

	for (size_t i = 0; i < SIZE; i++)
		result += v1(0, i) * v2(0, i);

	return
		result / (vectorLength(v1) * vectorLength(v2));

}
void buildTestMatrix(Matrix<>& A, Matrix<>& H, Matrix<>& lambda, Matrix<>& omega, double& e_l, Matrix<>& l_v, int index) {
	const int SIZE = A.rowsCount();
	
	// ������� � ������������ ���������� A �� ���������
	Matrix<> Lambda(SIZE);
	for (int i = 0; i < SIZE; i++) {
		Lambda(i, i) = lambda(0, i);
	}

	// ��������� ������ 
	normalize(omega);

	// ��������� ������� �����������
	H = Matrix<double>(SIZE);
	for (int row = 0; row < H.rowsCount(); row++) {
		for (int col = 0; col < H.colsCount(); col++) {
			H(row, col) = double(1 - 2 * omega(0, col) * omega(0, row));
		}
	}

	// ���������� ������������� �������
	A = H * Lambda * H.transpose();

	// ����� ������������ �� ������ ������. �������� � �����. ��� �������
	e_l = lambda(0, index);
	for (int i = 0; i < SIZE; i++) {
		if (abs(e_l) > abs(lambda(0, i))) {
			e_l = lambda(0, i);
			index = i;
		}
	}
	l_v = H.column(index);

	/*for (int i = 0; i < SIZE; i++) {
		cout << A * H.column(i) << endl;
	}
	cout << "=============" << endl;
	for (int i = 0; i < SIZE; i++) {
		cout << H.column(i) * lambda[i] << endl;
	}*/
}
void writeToFile(Matrix<>& A, double e_l, double e_v, double iterations, string path) {
	ofstream out;
	out.open(path);
	if (out.is_open()) {
		out << A.rowsCount() << endl;
		out << A;
		out << e_l << endl;
		out << e_v << endl;
		out << iterations;
	}
}
bool findY(Matrix<> B, Matrix<> f, Matrix<>& y) {
	const int SIZE = B.colsCount();
	for (int i = 0; i < SIZE; i++) {
		double sum = f(0, i);
		for (int j = 0; j <= i - 1; j++) {
			sum -= B(i, j) * y(0, j);
		}
		if (B(i, i) == 0) {
			// Получили 0 на главной диагонали, выходим из метода
			return false;
		}
		y(0, i) = sum / B(i, i);
	}
	// Вектор полностью обработан, возврат true
	return true;
}
void findX(Matrix<> C, Matrix<> y, Matrix<> x) {
	const int SIZE = C.colsCount();
	for (int i = SIZE - 1; i >= 0; i--) {
		double sum = y(0, i);
		for (int j = i + 1; j < SIZE; j++) {
			sum -= C(i, j) * x(0, j);
		}
		x(0, i) = sum;
	}
}
double vectProd(Matrix<>& v1, Matrix<>& v2) {
	const int SIZE = v1.colsCount();
	double res = 0;
	for (int i = 0; i < SIZE; i++) {
		res += v1(0, i) * v2(0, i);
	}
	return res;
}
void method(Matrix<>& A, Matrix<>& xn, int M, double eps_l, double eps_v, int& k_l, int& k_v, double lambda) {
	const int SIZE = A.colsCount();

	Matrix<> f_1(1, SIZE);
	Matrix<> f_2(1, SIZE);
	Matrix<> B(SIZE);			// Нижнетреугольная матрица LU-разложения
	Matrix<> C(SIZE);			// Верхнетреугольная матрица LU-разложения
	Matrix<> E(SIZE, SIZE, 1);	// Единичная матрица
	Matrix<> e(1, SIZE, 1);		// Единичный вектор

	bool continueToWork = Khaletsky(A, B, C);
	int count = 0;
	double a1 = 1, a2;

	while (continueToWork) {
		Matrix<> y(1, SIZE), x(1, SIZE);
		if (findY(B, f_1, y)) {
			findX(C, y, x);
		}

		if (count >= 2) {
			a2 = a1;
		}
		a1 = vectProd(f_1, x);

		if (count >= 2) {
			f_2 = f_1;
		}
		f_1 = x;
		normalize(f_1);

		count++;
		if (count >= 3) {
			if (abs(a1 - a2) < eps_l) {
				k_l = count;
			}

			if (abs(cosBetweenVectors(f_1, f_2)) < eps_v) {
				k_v = count;
				if (k_l == -1) {
					k_l = count;
				}
				continueToWork = false;
			}
		}

		if (count >= M) {
			continueToWork = false;
		}
	}

	if (k_v != -1) {
		lambda = 1 / a2;
		xn = f_1;
	}
}