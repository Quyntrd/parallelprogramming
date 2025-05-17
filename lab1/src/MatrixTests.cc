#include <Matrix.h>
#include <chrono>
#include <iostream>
#include <string>
#include <sstream>
#include <ctime>
#include <cstdlib>

#define COUNT_TIMES 10

using namespace std;
using namespace std::chrono;

int main() {
    srand(static_cast<unsigned>(time(nullptr)));

    for (unsigned int i = 100; i <= 1000; i += 100) {
        double total_duration = 0.0;
        cout << "Console :::: Multiplying matrices of size " << i << "x" << i << "..." << endl;

        for (unsigned int j = 0; j < COUNT_TIMES; ++j) {
            Matrix a(i, i, 2, 100);
            Matrix b(i, i, 2, 100);

            const string path = "C:/Users/ROfl/Desktop/parallelprogramming/lab1/tests/";

            ostringstream a_file, b_file, c_file;
            a_file << path << "a" << i << ".txt";
            b_file << path << "b" << i << ".txt";
            c_file << path << "c" << i << ".txt";

            a.WriteData(a_file.str());
            b.WriteData(b_file.str());

            auto start_time = high_resolution_clock::now();
            Matrix result = a * b;
            auto end_time = high_resolution_clock::now();

            total_duration += duration_cast<microseconds>(end_time - start_time).count() / 1e6;

            result.WriteData(c_file.str());
        }

        double avg_duration = total_duration / COUNT_TIMES;
        cout << "Console :::: Average multiplication time: " << avg_duration << " seconds." << endl;
    }

    system("pause");
    return 0;
}
