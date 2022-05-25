#pragma once
#include <vector>
#include <iostream>
using namespace std;

class Matrix
{
public:
	int m, n;
	std::vector<double> array;

	Matrix(int m, int n);
	Matrix(double (*func) (int, int), int m, int n);
	Matrix(std::vector<double> array, int n);

	Matrix Gauss(vector<double>& x);
	Matrix Gauss();
	Matrix Inverse();
	/*Получение элемента по номеру строки и столбца*/
	double elem(int x, int y);
	/*Замена элемента по номеру строки и столбца*/
	void replace_elem(int x, int y, double source);
	/*Добавление столбца с правой стороны*/
	void addColumn(std::vector<double> b);
	void deleteColumn(int n);

	vector<double> getColumn(int n);
	double detDiag();
	friend ostream& operator << (ostream& stream, Matrix matr);
};

ostream& operator << (ostream& stream, Matrix matr);