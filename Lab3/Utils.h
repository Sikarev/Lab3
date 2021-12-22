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
void normalize(vector<double>& vec) {
	double len = sqrt(norm_of_vector(vec));
	// Нормируем вектор
	for (int i = 0; i < vec.size(); i++) {
		vec[i] /= len;
	}
};
inline double norm_of_vector(const vector<double>& lhs)
{

	double result = 0;

	for (auto& vector_element : lhs)
	{
		result += vector_element * vector_element;
	}

	return sqrt(result);

}
inline double cos_phi_between_vectors(const vector<double>& lhs, const vector<double>& rhs)
{
	if (lhs.size() != rhs.size())
		return -1;

	double result = 0;
	const size_t n = lhs.size();

	for (size_t i = 0; i < n; i++)
		result += lhs[i] * rhs[i];

	return
		result / (norm_of_vector(lhs) * norm_of_vector(rhs));

}
void buildTestMatrix(Matrix<>& A, Matrix<>& H, vector<double> lambda, vector<double> omega, double& e_l, Matrix<>& l_v) {
	const int SIZE = A.rowsCount();
	
	// ������� � ������������ ���������� A �� ���������
	Matrix<> Lambda(SIZE);
	for (int i = 0; i < SIZE; i++) {
		Lambda(i, i) = lambda[i];
	}

	// ��������� ������ 
	normalize(omega);

	// ��������� ������� �����������
	H = Matrix<double>(SIZE);
	for (int row = 0; row < H.rowsCount(); row++) {
		for (int col = 0; col < H.colsCount(); col++) {
			H(row, col) = double(1 - 2 * omega[col] * omega[row]);
		}
	}

	// ���������� ������������� �������
	A = H * Lambda * H.transpose();

	// ����� ������������ �� ������ ������. �������� � �����. ��� �������
	e_l = lambda[index];
	for (int i = 1; i < lambda.size(); i++) {
		if (abs(e_l) > abs(lambda[i])) {
			e_l = lambda[i];
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
void method(Matrix<>& A, double l_1, Matrix<>& x_1, int M) {
	const int SIZE = A.colsCount();

	Matrix<> fu(1, SIZE);
	Matrix<> fu_2(1, SIZE);
	Matrix<> B(SIZE);			// Нижнетреугольная матрица LU-разложения
	Matrix<> C(SIZE);			// Верхнетреугольная матрица LU-разложения
	Matrix<> E(SIZE, SIZE, 1);	// Единичная матрица
	Matrix<> e(1, SIZE, 1);		// Единичный вектор

	bool keepWorking = Khaletsky(A, B, C);
}