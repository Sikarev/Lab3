#include <iostream>
#include <vector>
#include <string>
#include "Utils.h"

using namespace std;

int main() {
	string path = "data.txt";

	const int SIZE = 3;
	Matrix<>A(SIZE), H(SIZE);

	vector<double> lambda;
	for (int i = 0; i < SIZE; i++) {
		lambda.push_back(i + 1);
	}
	vector<double> omega;
	for (int i = 0; i < SIZE; i++) {
		omega.push_back(double(rand() % 10 + 1));
	}

	Matrix<>l_v(SIZE, 1);

	double e_v = 0.1;
	double e_l = 0.1;
	double iterations = 40;
	///////////////
	buildTestMatrix(A, H, lambda, omega, e_l, l_v);
	writeToFile(A, e_l, e_v, iterations, path);
	
	system("pause");
	return 0;
}