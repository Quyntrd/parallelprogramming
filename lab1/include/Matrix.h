#pragma once
#include <iostream>
#include <iomanip>
#include <vector>
#include <fstream>

using namespace std;

class Matrix {
	unsigned int _rows;
	unsigned int _cols;
	vector<vector<int>> _data;
public:
	Matrix();
	Matrix(unsigned int rows, unsigned int cols);
	Matrix(unsigned int rows, unsigned int cols, const int lower, const int upper);
	Matrix operator*(const Matrix& other) const;

	const vector<vector<int>>& Data() const { return _data; }
	vector<vector<int>>& Data() { return _data; }

	void ReadData(const string& filename);
	void WriteData(const string& filename) const;

	friend ostream& operator<<(ostream& os, const Matrix& matrix) {
		for (unsigned int i = 0; i < matrix._rows; i++) {
			for (unsigned int j = 0; j < matrix._cols; j++) {
				os << fixed << setprecision(4) << matrix._data[i][j] << ' ';
			}
			os << '\n';
		}
		return os;
	}

};

Matrix::Matrix() : _rows(0), _cols(0) {
	_data.resize(_rows, vector<int>(_cols, 0));
}

Matrix::Matrix(unsigned int rows, unsigned int cols) : _rows(rows), _cols(cols) {
	_data.resize(_rows, vector<int>(_cols, 0));
}

Matrix::Matrix(unsigned int rows, unsigned int cols, const int lower, const int upper) {
	_rows = rows;
	_cols = cols;
	_data.resize(_rows, vector<int>(_cols));

	srand(static_cast<unsigned>(time(nullptr)));

	for (unsigned int i = 0; i < _rows; ++i) {
		for (unsigned int j = 0; j < _cols; ++j) {
			_data[i][j] = lower + static_cast<int>(rand()) / (static_cast<float>(RAND_MAX) / (upper - lower));
		}
	}
}

void Matrix::ReadData(const string& filename) {
	ifstream fin(filename);
	if (!fin) {
		cerr << "Error during opening file in Read: " << filename << endl;
		return;
	}

	if (!(fin >> _rows >> _cols)) {
		cerr << "Error reading matrix dimensions from file: " << filename << endl;
		return;
	}

	_data.assign(_rows, vector<int>(_cols));

	for (unsigned int i = 0; i < _rows; i++) {
		for (unsigned int j = 0; j < _cols; j++)
		{
			fin >> _data[i][j];
		}
	}
}
void Matrix::WriteData(const string& filename) const {
	ofstream fout(filename);
	if (!fout) {
		cerr << "Error during opening file in Write: " << filename << endl;
		return;
	}
	fout << _rows << " " << _cols << std::endl;
	for (unsigned int i = 0; i < _rows; i++) {
		for (unsigned int j = 0; j < _cols; j++) {
			fout << fixed << setprecision(4) << _data[i][j] << ' ';
		}
		fout << '\n';
	}
}
Matrix Matrix::operator*(const Matrix& other) const {
	Matrix result(_rows, other._cols);

	for (unsigned int i = 0; i < _rows; ++i) {
		for (unsigned int j = 0; j < other._cols; ++j) {
			for (unsigned int k = 0; k < _cols; ++k) {
				result._data[i][j] += _data[i][k] * other._data[k][j];
			}
		}
	}
	return result;

}