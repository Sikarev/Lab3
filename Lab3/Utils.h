#pragma once
#include <vector>
#include <fstream>
#include <string>
#include "Matrix.h"

inline bool khaletsky(
	Matrix<>& A,
	Matrix<>& B, 
	Matrix<>& C
)
{
	const int size = A.rows_count();
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
			if (B(j, j) == 0.0) {
				// Метод Халецкого здесь неприменим
				return false;
			}
			C(j, i) = (A(j, i) - sum) / B(j, j);
		}
	}
	// Решено успешно
	return true;
}

inline void normalize(vector<double>& vector) {
	// Длина вектора
	double len = 0;
	for (const double vector_element : vector)
	{
		len += vector_element * vector_element;
	}
	len = sqrt(len);
	// Нормируем вектор
	for (double& vector_element : vector)
	{
		vector_element /= len;
	}
};
void buildTestMatrix(
	Matrix<>& A,
	Matrix<>& H,
	const vector<double>& lambda, 
	vector<double> omega, 
	double& e_l, 
	Matrix<>& l_v
) {
	const int size = A.rows_count();
	
	// Матрица с собственными занчениями A на диагонали
	Matrix<> Lambda_diagonal(size);
	for (int i = 0; i < size; i++) {
		Lambda_diagonal(i, i) = lambda[i];
	}

	// Нормируем вектор 
	normalize(omega);

	// Посроение матрицы Хаусхолдера
	H = Matrix<double>(size);
	for (int row = 0; row < H.rows_count(); row++) {
		for (int col = 0; col < H.cols_count(); col++) {
			H(row, col) = 1 - 2 * omega[col] * omega[row];
		}
	}

	// Построение тестировочной матрицы
	A = H * Lambda_diagonal * H.transpose();

	// Индекс минимального по модулю собств. значения и соотв. ему вектора
	int index = 0;
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
void writeToFile(
	Matrix<>& a,
	const double e_l,
	const double e_v,
	const double iterations,
	const string& path
) {
	ofstream out;
	out.open(path);
	if (out.is_open()) {
		out << a.rows_count() << endl;
		out << a;
		out << e_l << endl;
		out << e_v << endl;
		out << iterations;
	}
}

inline double norm_of_vector(const vector<double>& lhs)
{

	double result = 0;

	for (auto& vector_element : lhs)
	{
		result += vector_element * vector_element;
	}

	return sqrt(result);

}

inline double cos_phi_between_vectors(const vector<double> &lhs, const vector<double> &rhs)
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
