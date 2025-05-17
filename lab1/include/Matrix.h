#pragma once
#include <iostream>
#include <iomanip>
#include <vector>
#include <fstream>
#include <stdexcept>

using namespace std;

#ifdef __CUDACC__
#include <cuda_runtime.h>
// CUDA error checking helper
template<typename T>
inline void checkCuda(T err, const char* msg) {
    if (err != cudaSuccess) {
        throw runtime_error(string(msg) + ": " + cudaGetErrorString(err));
    }
}

// CUDA kernel: каждый поток вычисляет один элемент C
__global__ void matMulKernel(const int* A, const int* B, int* C,
    unsigned int A_rows, unsigned int A_cols,
    unsigned int B_cols) {
    unsigned int row = blockIdx.y * blockDim.y + threadIdx.y;
    unsigned int col = blockIdx.x * blockDim.x + threadIdx.x;
    if (row < A_rows && col < B_cols) {
        int sum = 0;
        for (unsigned int k = 0; k < A_cols; ++k) {
            sum += A[row * A_cols + k] * B[k * B_cols + col];
        }
        C[row * B_cols + col] = sum;
    }
}
#endif

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
        for (unsigned int j = 0; j < _cols; j++) {
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
    fout << _rows << " " << _cols << endl;
    for (unsigned int i = 0; i < _rows; i++) {
        for (unsigned int j = 0; j < _cols; j++) {
            fout << fixed << setprecision(4) << _data[i][j] << ' ';
        }
        fout << '\n';
    }
}

Matrix Matrix::operator*(const Matrix& other) const {
    if (_cols != other._rows) throw runtime_error("Matrix sizes invalid for multiplication");

#ifdef __CUDACC__
    // GPU implementation
    Matrix result(_rows, other._cols);
    size_t sizeA = _rows * _cols * sizeof(int);
    size_t sizeB = other._rows * other._cols * sizeof(int);
    size_t sizeC = _rows * other._cols * sizeof(int);

    int* d_A = nullptr, * d_B = nullptr, * d_C = nullptr;
    checkCuda(cudaMalloc(&d_A, sizeA), "cudaMalloc A");
    checkCuda(cudaMalloc(&d_B, sizeB), "cudaMalloc B");
    checkCuda(cudaMalloc(&d_C, sizeC), "cudaMalloc C");

    // Flatten A and B
    vector<int> flatA(_rows * _cols), flatB(other._rows * other._cols);
    for (unsigned int i = 0; i < _rows; ++i)
        for (unsigned int j = 0; j < _cols; ++j)
            flatA[i * _cols + j] = _data[i][j];
    for (unsigned int i = 0; i < other._rows; ++i)
        for (unsigned int j = 0; j < other._cols; ++j)
            flatB[i * other._cols + j] = other._data[i][j];

    checkCuda(cudaMemcpy(d_A, flatA.data(), sizeA, cudaMemcpyHostToDevice), "H2D A");
    checkCuda(cudaMemcpy(d_B, flatB.data(), sizeB, cudaMemcpyHostToDevice), "H2D B");

    dim3 block(16, 16);
    dim3 grid((other._cols + 15) / 16, (_rows + 15) / 16);
    matMulKernel << <grid, block >> > (d_A, d_B, d_C, _rows, _cols, other._cols);
    checkCuda(cudaGetLastError(), "Kernel launch");

    vector<int> flatC(_rows * other._cols);
    checkCuda(cudaMemcpy(flatC.data(), d_C, sizeC, cudaMemcpyDeviceToHost), "D2H C");

    for (unsigned int i = 0; i < _rows; ++i)
        for (unsigned int j = 0; j < other._cols; ++j)
            result._data[i][j] = flatC[i * other._cols + j];

    cudaFree(d_A);
    cudaFree(d_B);
    cudaFree(d_C);
    return result;
#else
    // CPU fallback
    Matrix result(_rows, other._cols);
    for (unsigned int i = 0; i < _rows; ++i)
        for (unsigned int j = 0; j < other._cols; ++j) {
            int sum = 0;
            for (unsigned int k = 0; k < _cols; ++k)
                sum += _data[i][k] * other._data[k][j];
            result._data[i][j] = sum;
        }
    return result;
#endif
}
