#include <Matrix.h>
#include <chrono>
#include <format>
#include <iostream>
#include <ctime>
#include <cstdlib>

#define COUNT_TIMES 10

using namespace std;
using namespace chrono;

int main() {
    srand(time(NULL));
    for (unsigned int i = 100; i <= 1000; i += 100) {
        double total_duration = 0.0;
        cout << "Console :::: Multiplying matrices of size " << i << "x" << i << "..." << endl;

        for (unsigned int j = 0; j < COUNT_TIMES; ++j) {
            Matrix a(i, i, 2, 100);
            Matrix b(i, i, 2, 100);

            string path = "C:/Users/Quyntrd/Desktop/parallel/parallelprogramming/lab1/tests/";
            a.WriteData(format("{}a{}.txt", path, i));
            b.WriteData(format("{}b{}.txt", path, i));

            auto start_time = high_resolution_clock::now();
            Matrix result = a * b;
            auto end_time = high_resolution_clock::now();

            total_duration += duration_cast<microseconds>(end_time - start_time).count() / 1e6;

            result.WriteData(format("{}c{}.txt", path, i));
        }

        double avg_duration = total_duration / COUNT_TIMES;
        cout << "Console :::: Average multiplication time: " << avg_duration << " seconds." << endl;
    }
    system("pause");
    return 0;
}