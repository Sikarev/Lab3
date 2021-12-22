#pragma once
#include <iostream>
#include <vector>
#include "Matrix.h"

using namespace std;

class LinearSystem {
public:
	LinearSystem(int SIZE = 3) : size(size) {
		// ������ ������ �� ���������� ����������
		Matrix<> Lambda(SIZE);
		for (int i = 0; i < SIZE; i++) {
			Lambda(i, i) = double(i + 1);
			l.push_back(i + 1);
		}

		// ������ ��������� ������ omega � ��������� ���
		vector<double> omega;
		double omegaLen = 0;
		for (int i = 0; i < SIZE; i++) {
			omega.push_back(double(rand() % 10) + 1);
			omegaLen += omega[i] * omega[i];
		}
		omegaLen = sqrt(omegaLen);
		for (int i = 0; i < omega.size(); i++) {
			omega[i] /= omegaLen;
		}

		// ��������� ������� �����������
		H = Matrix<double>(SIZE);
		for (int row = 0; row < H.rowsCount(); row++) {
			for (int col = 0; col < H.colsCount(); col++) {
				H(row, col) = double(1 - 2 * omega[row] * omega[col]);
			}
		}

		A = H * Lambda * H.transpose();
	}

	Matrix<> A;					// ������� �������
	Matrix<> H;					// ������� ����������� �������� ������� Ax
	vector<double> l;			// ����������� �������� ������� A
	int size;
};