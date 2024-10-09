#include <iostream>
#include <iomanip>
#include <ctime>
#include <cmath>

using namespace std;

const int MIN_EXPONENT = 1; 
const int MAX_EXPONENT = 17; 

// c o s t e : \ Theta (2ˆ n )
unsigned long pow2_1(unsigned n) {
    if (n == 0) {
        return 1;
    } else {
        return pow2_1(n - 1) + pow2_1(n - 1);
    }
}

// c o s t e : \ Theta (log n)
unsigned long pow2_2(unsigned n) {
    if (n == 0) {
        return 1;
    }
    int res = pow2_2(n / 2);
    if (n % 2 == 0) {
        return res * res;
    } else {
        return 2 * res * res;
    }
}

// c o s t e : \ Theta (n)
unsigned long pow2_3(unsigned n) {
    unsigned long res = 1;
    for (unsigned i = 1; i <= n; ++i) {
        res += res; 
    }
    return res;
}

int main() {
    cout << setw(15) << left << "# Exponente" << setw(20) << "Tiempo (pow2_1)" << setw(20) << "Tiempo (pow2_2)" << setw(20) << "Tiempo (pow2_3)" << endl;

    for (int N = MIN_EXPONENT; N <= MAX_EXPONENT; ++N) {
        double tiempoTotal1 = 0.0;
        double tiempoTotal2 = 0.0;
        double tiempoTotal3 = 0.0;

        for (int trial = 1; trial <= 30; ++trial) {
            auto start1 = clock();
            unsigned long result1 = pow2_1(N);
            auto end1 = clock();

            auto start2 = clock();
            unsigned long result2 = pow2_2(N);
            auto end2 = clock();

            auto start3 = clock();
            unsigned long result3 = pow2_3(N);
            auto end3 = clock();

            
            double expected_result = pow(2, N);

            if (result1 != expected_result || result2 != expected_result || result3 != expected_result) {
                cerr << "Error: Los resultados no coinciden para N = " << N << endl;
                return 1;
            }

            double tiempo1 = (end1 - start1) * 1000.0 / CLOCKS_PER_SEC;
            double tiempo2 = (end2 - start2) * 1000.0 / CLOCKS_PER_SEC;
            double tiempo3 = (end3 - start3) * 1000.0 / CLOCKS_PER_SEC;

            tiempoTotal1 += tiempo1;
            tiempoTotal2 += tiempo2;
            tiempoTotal3 += tiempo3;
        }

        double tiempoMedio1 = tiempoTotal1 / 30;
        double tiempoMedio2 = tiempoTotal2 / 30;
        double tiempoMedio3 = tiempoTotal3 / 30;

        cout << setw(15) << left << N << fixed << setprecision(5) << setw(20) << tiempoMedio1 << setw(20) << tiempoMedio2 << setw(20) << tiempoMedio3 << endl;
    }

    return 0;
}
