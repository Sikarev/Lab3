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
				// ����� ��������� ����� ����������
				return false;
			}
			C(j, i) = (A(j, i) - sum) / B(j, j);
		}
	}
	// ������ �������
	return true;
}
void normalize(vector<double>& vec) {
	// ����� �������
	double len = 0;
	for (int i = 0; i < vec.size(); i++) {
		len += vec[i] * vec[i];
	}
	len = sqrt(len);
	// ��������� ������
	for (int i = 0; i < vec.size(); i++) {
		vec[i] /= len;
	}
};
void buildTestMatrix(Matrix<>& A, Matrix<>& H, vector<double> lambda, vector<double> omega, double& e_l, Matrix<>& l_v, int& index) {
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
	Matrix<> B(SIZE);			// ���������������� ������� LU-����������
	Matrix<> C(SIZE);			// ����������������� ������� LU-����������
	Matrix<> E(SIZE, SIZE, 1);	// ��������� �������
	Matrix<> e(1, SIZE, 1);		// ��������� ������

	bool keepWorking = Khaletsky(A, B, C);
}