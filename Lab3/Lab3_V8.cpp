#include <iostream>
#include <vector>
#include <string>
#include "Utils.h"

using namespace std;

int main() {
	string path = "data.txt";

	const int SIZE = 3;
	Matrix<>A(SIZE), H(SIZE);

	Matrix<double> lambda(1, SIZE);
	for (int i = 0; i < SIZE; i++) {
		lambda(0, i) = i + 1;
	}
	Matrix<double> omega(1, SIZE);
	for (int i = 0; i < SIZE; i++) {
		omega(0, i) = (double(rand() % 10 + 1));
	}

	Matrix<>l_v(SIZE, 1);

	double eps_v = 0.1;
	double eps_l = 0.1;
	int index = 0, iterations = 40, k_v = -1, k_l = -1;
	///////////////
	buildTestMatrix(A, H, lambda, omega, eps_l, l_v, index);
	writeToFile(A, eps_l, eps_v, iterations, path);

	double l = 0;

	Matrix<> xn(1, SIZE);

	method(A, xn, iterations, eps_l, eps_v, k_l, k_v, l);
	
	system("pause");
	return 0;
}