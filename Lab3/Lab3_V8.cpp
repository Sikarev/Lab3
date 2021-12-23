#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include "Utils.h"

using namespace std;

int main() {
	srand(time(0));

	const int SIZE = 10;

	string path = "data.txt";
	string output = "output.txt";

	double eps_v, eps_l, r;
	eps_v = eps_l = pow(10, -5);
	int index = 0, iterations = 1000, k_v = -1, k_l = -1;
	
	///////////////

	int begin = -3;
	int end = 3;

	// �������� �������
	Matrix<>A(SIZE), H(SIZE);
	// ������ ����������� ��������
	Matrix<double> lambda(1, SIZE);

	buildTestMatrix(A, H, lambda, begin, end);

	// ���������� �������� ������ � ����
	writeToFile(A, eps_l, eps_v, iterations, path);

	double l = 0;
	int count = 0;
	Matrix<> xn(1, SIZE);

	// ������� ������ ������� �������� �������� � �������������
	method(A, xn, iterations, eps_l, eps_v, k_l, k_v, l);

	int iterationCount = k_l > k_v ? k_l : k_v;

	// ���������� ���� �������� ������� ������������ ������
	precisionMeasure(A, xn, l, r);

	// ������ ����������� � ����
	writeResults(xn, l, iterationCount, r, output);
	
	system("pause");
	return 0;
}