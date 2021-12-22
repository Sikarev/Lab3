#include <iostream>
#include <vector>
#include <string>
#include "Utils.h"

using namespace std;

int main() {
	const string path = "data.txt";

	constexpr int size = 3;
	Matrix<>a(size), h(size);

	vector<double> lambda;
	for (int i = 0; i < size; i++) {
		lambda.push_back(static_cast<double>(i) + 1.0);
	}
	vector<double> omega;
	for (int i = 0; i < size; i++) {
		omega.push_back(rand() % 10 + 1.0);
	}

	Matrix<>l_v(size, 1);

	constexpr double e_v = 0.1;
	double e_l = 0.1;
	constexpr double iterations = 40;
	///////////////
	buildTestMatrix(a, h, lambda, omega, e_l, l_v);
	writeToFile(a, e_l, e_v, iterations, path);
	
	system("pause");
	return 0;
}