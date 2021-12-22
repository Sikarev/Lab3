#pragma once
#include <iostream>
#include <vector>
#include "Matrix.h"

using namespace std;

class linear_system {
public:
	explicit linear_system(const int size = 3) : size(size) {
		// Создаём вектор со случайными значениями
		Matrix<> lambda(size);
		for (int i = 0; i < size; i++) {
			lambda(i, i) = static_cast<double>(i) + 1.0;
			l.push_back(i + 1);
		}

		// Создаём случайный вектор omega и нормируем его
		vector<double> omega;
		double omega_len = 0;
		for (int i = 0; i < size; i++) {
			omega.push_back(static_cast<double>(rand() % 10) + 1);
			omega_len += omega[i] * omega[i];
		}
		omega_len = sqrt(omega_len);
		for (double& i : omega)
		{
			i /= omega_len;
		}

		// Посроение матрицы Хаусхолдера
		h = Matrix<double>(size);
		for (int row = 0; row < h.rows_count(); row++) {
			for (int col = 0; col < h.cols_count(); col++) {
				h(row, col) = 1 - 2 * omega[row] * omega[col];
			}
		}

		a = h * lambda * h.transpose();
	}

	Matrix<> a;					// Матрица системы
	Matrix<> h;					// Матрица собственных векторов системы Ax
	vector<double> l;			// Собственные значения матрицы A
	int size;
};