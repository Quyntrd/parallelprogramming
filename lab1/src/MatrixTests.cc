#include <Matrix.h>
#include <chrono>
#include <iostream>
#include <string>
#include <sstream>
#include <ctime>
#include <cstdlib>

#define COUNT_TIMES 10

using namespace std;
using namespace chrono;

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    for (unsigned int i = 100; i <= 1000; i += 100) {
        cout << "Console :::: Multiplying matrices of size " << i << "x" << i << "..." << endl;
        double total_duration = 0.0;

        for (unsigned int j = 0; j < COUNT_TIMES; ++j) {
            int matrix_size = i;
            MPI_Bcast(&matrix_size, 1, MPI_INT, 0, MPI_COMM_WORLD);

            Matrix a(matrix_size, matrix_size);
            Matrix b(matrix_size, matrix_size);

            if (rank == 0) {
                a = Matrix(matrix_size, matrix_size, 2.0f, 100.0f);
                b = Matrix(matrix_size, matrix_size, 2.0f, 100.0f);
            }

            MPI_Barrier(MPI_COMM_WORLD);
            auto start_time = high_resolution_clock::now();
            Matrix result = a * b;
            auto end_time = high_resolution_clock::now();

            if (rank == 0) {
                total_duration += duration_cast<microseconds>(end_time - start_time).count() / 1e6;
            }
        }

        if (rank == 0) {
            double avg_duration = total_duration / COUNT_TIMES;
            cout << "Console :::: Average multiplication time: " << avg_duration << " seconds." << endl;
        }
    }

    MPI_Finalize();
    system("pause");
    return 0;
}
