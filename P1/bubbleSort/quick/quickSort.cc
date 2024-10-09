//Mihai Alexandru Stefanoiu
#include <unistd.h>
#include <iostream>
#include <math.h>


using namespace std;

//--------------------------------------------------------------
// Middle QuickSort
void 
middle_QuickSort(int * v, long left, long right){

    long i,j;
    int pivot,aux; 
    if (left<right)
    {
        i=left; j=right;
        pivot=v[(i+j)/2];
        do
        {
            while (v[i]<pivot) i++;
            while (v[j]>pivot) j--;
            if (i<=j)
            {
                aux=v[i]; v[i]=v[j]; v[j]=aux;
                i++; j--;
            }
       } while (i<=j);
       if (left<j)  middle_QuickSort(v,left,j);
       if (i<right) middle_QuickSort(v,i,right);
    }
}

int main(void){
    unsigned int semilla = 0;
    srand(semilla);

    cout << "# QuickSort CPU times in milliseconds:"
        << endl
        << "# Size \t Average CPU time (ms.)"
        << endl
        << "# ---------------------------------"
        << endl;

    for (int exp = 15; exp <= 20; exp++){
        size_t size = size_t( pow(2,exp) );
        double tiempoTotal = 0;

        for (int trial = 1; trial <= 30; trial++) {
            int* v = new int [size];
            
            if (!v){
                cerr << "Error, not enough memory!" << endl;
                exit (EXIT_FAILURE);  
            }

            for (size_t j = 0; j < size; j++) 
                v[j] = rand(); 

            int left = 0;
            size_t right = size - 1;
            auto start = clock();
            middle_QuickSort(v, left, right);
            auto end = clock();
            double tiempo = end - start;
            tiempoTotal += tiempo;

            delete[] v;
        }

        double tiempoMedio = tiempoTotal / 30;
        cout << size << "\t\t" << 1000.0 * (tiempoMedio) / CLOCKS_PER_SEC << endl;
    }

    return 0;
}
