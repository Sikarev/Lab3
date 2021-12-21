#pragma once
#include <iostream>
#include <vector>
#include "Matrix.h"

using namespace std;

class LinearSystem {
public:
	LinearSystem(int SIZE = 3) : size(size) {
		// Создаём вектор со случайными значениями
		Matrix<> Lambda(SIZE);
		for (int i = 0; i < SIZE; i++) {
			Lambda(i, i) = double(i + 1);
			l.push_back(i + 1);
		}

		// Создаём случайный вектор omega и нормируем его
		vector<double> omega;
		double omegaLen = 0;
		for (int i = 0; i < SIZE; i++) {
			omega.push_back(double(rand() % 10) + 1);
			omegaLen += omega[i];
		}
		for (int i = 0; i < omega.size(); i++) {
			omega[i] /= omegaLen;
		}

		// Посроение матрицы Хаусхолдера
		H = Matrix<double>(SIZE);
		for (int row = 0; row < H.RowsCount(); row++) {
			for (int col = 0; col < H.ColsCount(); col++) {
				H(row, col) = double(1 - 2 * omega[row] * omega[col]);
			}
		}

		A = H * Lambda * H.transpose();
	}
private:
	Matrix<> A;					// Матрица системы
	Matrix<> H;					// Матрица собственных векторов системы Ax
	vector<double> l;			// Собственные значения матрицы A
	int size;
	void printMatrix(Matrix<> matr);
};

void LinearSystem::printMatrix(Matrix<> matr) {
	for (int i = 0; i < matr.RowsCount(); i++) {
		for (int j = 0; j < matr.ColsCount(); j++) {
			cout << matr(i, j) << '\t';
		}
		cout << endl;
	}
	cout << "------------------" << endl;
}