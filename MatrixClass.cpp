#pragma once
#include <vector>
#include "pch.h"
#include "MatrixClass.h"

using namespace std;

Matrix::Matrix(int m, int n)
{
	this->m = m;
	this->n = n;
	array.insert(array.cbegin(), m * n, 0.0);
}


Matrix::Matrix(double(*func)(int, int), int m, int n)
{
	this->m = m;
	this->n = n;
	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < n; j++)
		{
			array.push_back(func(i + 1, j + 1));
		}
	}
}

Matrix::Matrix(std::vector<double> array, int n)
{
	this->array = array;
	this->n = n;
	this->m = array.size() / n;
}

Matrix Matrix::Gauss(vector<double>& x){

	Matrix a = *this;
	int n = a.m;
	int m = a.n;

	for (int k = 1; k < n; k++)
	{
		double Amax = abs(a.elem(k, k));
		int p = k;

		for (int i = k + 1; i <= n; i++)
		{
			if (abs(a.elem(i, k)) > Amax)
			{
				Amax = abs(a.elem(i, k));
				p = i;
			}
		}

		for (int j = k; j <= m; j++)
		{
			double C = a.elem(k, j);
			a.replace_elem(k, j, a.elem(p, j));
			a.replace_elem(p, j, C);
		}

		for (int i = k + 1; i <= n; i++)
		{
			for (int j = k + 1; j <= m; j++)
			{
				a.replace_elem(
					i, j,
					a.elem(i, j) - (a.elem(i, k) * a.elem(k, j)) / a.elem(k, k)
					);
			}
		}
	}

	x.clear();
	x.resize(n);
	//vector<double> x(n);


	x[n - 1] = a.elem(n, n + 1) / a.elem(n, n);

	for (int i = n - 1; i >= 1; i--)
	{
		double s = a.elem(i, n + 1);
		for (int j = n; j >= i + 1; j--)
		{
			s = s - a.elem(i, j) * x[j - 1];
		}
		x[i - 1] = s / a.elem(i, i);
	}

	double determinant = 1;

	for (size_t i = 1; i <= a.m; i++)
	{
		determinant *= elem(i, i);
	}

	return a;
}
Matrix Matrix::Gauss(){

	Matrix a = *this;
	int n = a.m;
	int m = a.n;

	for (int k = 1; k < n; k++)
	{
		double Amax = abs(a.elem(k, k));
		int p = k;

		for (int i = k + 1; i <= n; i++)
		{
			if (abs(a.elem(i, k)) > Amax)
			{
				Amax = abs(a.elem(i, k));
				p = i;
			}
		}

		for (int j = k; j <= m; j++)
		{
			double C = a.elem(k, j);
			a.replace_elem(k, j, a.elem(p, j));
			a.replace_elem(p, j, C);
		}

		for (int i = k + 1; i <= n; i++)
		{
			for (int j = k + 1; j <= m; j++)
			{
				a.replace_elem(
					i, j,
					a.elem(i, j) - (a.elem(i, k) * a.elem(k, j)) / a.elem(k, k)
					);
			}
		}
	}

	return a;
}

//Matrix Matrix::Inverse(){
//	Matrix a = *this;
//
//	a.
//
//}

/* x - номер строки, y - номер столбца*/
double Matrix::elem(int x, int y)
{
	if (x > -1 && x <= this->n && y > -1 && y <= this->n)
		return array[(x - 1) * n + (y - 1)];
	else return 0.0;
}

void Matrix::replace_elem(int row, int column, double source)
{
	if ((row - 1) * n + (column - 1) < array.size())
		array[(row - 1) * n + (column - 1)] = source;
}

void Matrix::addColumn(std::vector<double> b)
{
	vector<double> array_2;
	for (int i = 0; i < m; i++)
	{
		array_2.insert(array_2.cbegin() + (i * (n + 1)), array.cbegin() + i * (n), array.cbegin() + i * (n)+(n));
		array_2.push_back(b[i]);
	}

	n++;
	array = array_2;
}


//void deleteColumn(int n){
//
//	minor.array.erase(
//		minor.array.begin() + minor.m * (i - 1), minor.array.begin() + (minor.m) * (i)
//		);
//	for (int k = 0; k < minor.n - 1; k++)
//	{
//		auto a = minor.array.cbegin();
//		a += (minor.m) * k + (j - 1) - k;
//		auto b = minor.array.cend();
//		if (a != b)
//			minor.array.erase(a);
//	}
//}

vector<double> Matrix::getColumn(int n)
{
	vector<double> a;
	for (size_t i = 1; i <= m; i++)
	{
		a.push_back(elem(i, n));
	}
	return a;
}

double Matrix::detDiag(){
	double determinant = 1;
	for (size_t i = 1; i <= m; i++)
	{
		determinant *= elem(i, i);
	}
	return determinant;
}

ostream& operator<<(ostream& stream, Matrix matr)
{
	for (int i = 0; i < matr.array.size(); i++)
	{
		stream.precision(4);
		stream << matr.array[i] << '\t';
		if ((i + 1) % (matr.n) == 0) stream << '\n';
	}
	return stream;
}
