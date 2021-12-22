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
			for (int k = 0; k < j - 1; k++) {
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
double vectorLength(Matrix<> v)
{
	const int SIZE = v.colsCount();
	double result = 0;

	for (int i = 0; i < SIZE; i++){
		result += v(0, i) * v(0, i);
	}
	return sqrt(result);

}
void normalize(Matrix<>& v) {
	const int SIZE = v.colsCount();
	double len = vectorLength(v);
	// Нормализуем вектор
	for (int i = 0; i < SIZE; i++) {
		v(0, i) /= len;
	}
};
double cosBetweenVectors(Matrix<> v1, Matrix<> v2)
{
	if (!v1.sameSizeWith(v2))
		return 99;

	double result = 0;
	const int SIZE = v1.colsCount();

	for (size_t i = 0; i < SIZE; i++)
		result += v1(0, i) * v2(0, i);

	return
		result / (vectorLength(v1) * vectorLength(v2));

}
void buildTestMatrix(Matrix<>& A, Matrix<>& H, Matrix<>& lambda, int begin, int end) {
	const int SIZE = A.rowsCount();

	// Заполняем матрицу случайными значениями из диапазона end-begin
	for (int i = 0; i < SIZE; i++) {
		lambda(0, i) = rand() % end + begin;
	}

	// Случайный нормированный вектор
	Matrix<double> omega(1, SIZE);
	for (int i = 0; i < SIZE; i++) {
		omega(0, i) = rand() % 2001 - 1000;
	}
	normalize(omega);
	
	// Диагональная матрица с собственными значениями матрицы А
	Matrix<> Lambda(SIZE);
	for (int i = 0; i < SIZE; i++) {
		Lambda(i, i) = lambda(0, i);
	}

	// Матрица Хаусхолдера
	H = Matrix<double>(SIZE);
	for (int row = 0; row < H.rowsCount(); row++) {
		for (int col = 0; col < H.colsCount(); col++) {
			H(row, col) = double(1 - 2 * omega(0, col) * omega(0, row));
		}
	}

	// Создание тестировочной матрицы
	A = H * Lambda * H.transpose();
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
bool findY(Matrix<>& B, Matrix<>& f, Matrix<>& y) {

	const int SIZE = B.colsCount();

	for (int i = 0; i < SIZE; i++) {
		double sum = f(0, i);
		for (int j = 0; j <= i - 1; j++) {
			sum -= B(i, j) * y(0, j);
		}
		if (B(i, i) == 0) {
			// Получили 0 на главной диагонали, выходим из метода
			return false;
		}
		y(0, i) = sum / B(i, i);
	}
	/*cout << y << "========" << endl;
	cout << B;*/
	// Вектор полностью обработан, возврат true
	return true;
}
void findX(Matrix<>& C, Matrix<>& y, Matrix<>& x) {
	const int SIZE = C.colsCount();
	for (int i = SIZE - 1; i >= 0; i--) {
		double sum = y(0, i);
		for (int j = i + 1; j < SIZE; j++) {
			sum -= C(i, j) * x(0, j);
		}
		x(0, i) = sum;
	}
}
double vectProd(Matrix<>& v1, Matrix<>& v2) {
	const int SIZE = v1.colsCount();
	double res = 0;
	for (int i = 0; i < SIZE; i++) {
		res += v1(0, i) * v2(0, i);
	}
	return res;
}
void method(Matrix<>& A, Matrix<>& xn, int M, double eps_l, double eps_v, int& k_l, int& k_v, double& lambda) {
	const int SIZE = A.colsCount();

	Matrix<> f_1(1, SIZE, 1);
	Matrix<> f_2(1, SIZE);
	Matrix<> B(SIZE);			// Нижнетреугольная матрица LU-разложения
	Matrix<> C(SIZE);			// Верхнетреугольная матрица LU-разложения
	Matrix<> E(SIZE, SIZE, 1);	// Единичная матрица
	Matrix<> e(1, SIZE, 1);		// Единичный вектор


	// Производим LU-разложение, в случае успеха запускаем процесс итерации (строка 165)
	bool continueToWork = Khaletsky(A, B, C);
	int count = 0;
	double a1 = 1, a2;

	while (continueToWork) {
		Matrix<> y(1, SIZE), x(1, SIZE);

		// Если удаось найти Y, ищем X
		if (findY(B, f_1, y)) {
			findX(C, y, x);
		}

		// Запоминаем предудущее собственное значение, начиная со второй итерации
		if (count >= 2) {
			a2 = a1;
		}
		a1 = vectProd(f_1, x);

		// Запоминаем предыдущее значение собственного вектора, начиная со второй итерации
		if (count >= 2) {
			f_2 = f_1;
		}
		f_1 = x;

		// Нормируем вектор
		normalize(f_1);

		count++;

		// Начиная с третьей итерации
		if (count >= 3) {
			// Если мера собственный достигла требуемой точности, запоминаем потребовавшееся количетсво итераций
			if (abs(a1 - a2) < eps_l) {
				k_l = count;
			}

			// Если угол между векторами достиг требуемой точности, запоминаем потребовавшееся количетсво итераций
			if (abs(cosBetweenVectors(f_1, f_2)) < eps_v) {
				k_v = count;
				if (k_l == -1) {
					k_l = count;
				}
				// Завершаем процесс итерации
				continueToWork = false;
			}
		}

		// Завершаем процесс итераций, если превышено максимальное их количество
		if (count >= M) {
			continueToWork = false;
		}
	}

	// Если удалось достичь требуемой точности, записываем полученные 
	if (k_v != -1) {
		lambda = 1 / a2;
		xn = f_1;
	}
}
// Поиск максимального по модулю отклонение компоненты вектора Ax − λx от нуля
void precisionMeasure(Matrix<>& A, Matrix<>& x, double l, double& r) {
	const int SIZE = A.colsCount();

	Matrix<> b(1, SIZE);

	b = A * x;

	// Записываем отклонени я векторов Ax − λx от нуля
	for (int i = 0; i < SIZE; i++) {
		b(0, i) = b(0, i) - l * x(0, i);
	}

	r = b(0, 0);

	// Находим максимальное отклонение среди полученных
	for (int i = 1; i < SIZE; i++) {
		if (b(0, i) > r) {
			r = b(0, i);
		}
	}
}
void writeResults(Matrix<>& x, double l, double count, double r, string path) {
	ofstream out;
	out.open(path);
	if (out.is_open()) {
		out << l << endl;
		out << x;
		out << count << endl;
		out << r << endl;
	}
}