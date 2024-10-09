//MIHAI ALEXANDRU STEFANOIU 
/*
ADA. 2023-24
Práctica 2:"Empirical analysis by means of program-steps account of two sorting algorithms: Middle-Quicksort and Heapsort."
*/
#include <unistd.h>
#include <iostream>
#include <math.h>
#include <iomanip>



using namespace std;

unsigned long _STEPS_; //VARIABLE GLOBAL

//------------------------------------------------------------
void reverseArray(int* vec, int size) {
    int start = 0;
    int end = size - 1;

    while (start < end) {
        // Intercambiar los elementos en las posiciones start y end
        int temp = vec[start];
        vec[start] = vec[end];
        vec[end] = temp;

        // Mover los índices start y end hacia adentro
        start++;
        end--;
    }
}

//--------------------------------------------------------------
// Middle Quick Sort
void 
middle_QuickSort(int * v, long left, long right){ //left = 0, right = tam -1
long i,j;
int pivot; 
	if (left<right){
		i=left; j=right;
		pivot=v[(i+j)/2];
		do{
			while (v[i]<pivot) {
                
                i++; //ESTOS 2 BUCLES TAMBIEN AUMENTAN CON EL TAMAÑO DEL PROBLEMA
                _STEPS_++;

            }
			while (v[j]>pivot){
                
                 j--;
                _STEPS_++;
            }
			if (i<=j){ 
                _STEPS_++;
				swap(v[i], v[j]);
				i++; j--;
			}
		}while (i<=j);//ESTO SI Q CRECE CON EL TAMAÑO DEL PROBLEMA, MAYOR ARRAY MAYORES I Y J


		if (left < j){
            
            middle_QuickSort(v,left,j);

        }
		if (i < right){
            
            middle_QuickSort(v,i,right);
        }
	}
}

//--------------------------------------------------------------                
//HEAPSORT
// Procedure sink used by Heapsort algorith
// Sink an element (indexed by i) in a tree to keep the heap property. n is the size of the heap. 

void sink(int *v, size_t n, size_t i)
{
size_t largest;
size_t l, r; //left and right child

do{
    largest = i;  // Initialize largest as root
    l = 2*i + 1;  // left = 2*i + 1
    r = 2*i + 2;  // right = 2*i + 2
 
    // Is left child larger than root?
    if (l < n && v[l] > v[largest])
        largest = l;
 
    // Is right child larger than largest so far
    if (r < n && v[r] > v[largest])
        largest = r;
 
    // If largest is still root then the process is done
    if (largest == i) break;
    
	// If not, swap new largest with current node i and repeat the process with childs. 
    swap(v[i], v[largest]);
    i=largest;  
    _STEPS_++;
   } while (true);
}
 
//--------------------------------------------------------------                
// Heapsort algorithm (ascending sorting)
void heapSort(int *v, size_t n)
{
    // Build a MAX-HEAP with the input array
    for (size_t  i = n / 2 - 1; i >= 0; i--){
        sink(v, n, i);
                _STEPS_++;

        if (i==0) break; //as size_t is unsigned type
	}	

  	// A this point we have a HEAP on the input array, let's take advantage of this to sort the array:
    // One by swap the first element, which is the largest (max-heap), with the last element of the vector and rebuild heap by sinking the new placed element on the begin of vector.  
    for (size_t i=n-1; i>0; i--)
    {
		// Move current root to the end.
        swap(v[0], v[i]);
 		// Now largest element is at the end but do not know if the element just moved to the beginning is well placed, so a sink process is required.
		// Note that, at the same time, the HEAP size is reduced one element by one.
        sink(v, i, 0);
		// The procedure ends when HEAP has only one element, the lower of the input array
                _STEPS_++;

    }
}






int main(void) {
    //srand(0);
    srand(time(NULL));

    int numSamples = 30;
    
    cout << "#QUICKSORT VERSUS HEAPSORT." << endl
         << "#Average processing Msteps (millions of program steps)" << endl
         << "#Number of samples (arrays of integer): 30" << endl << endl
         << "#\t\t  RANDOM ARRAYS\t      SORTED ARRAYS \tREVERSE SORTED ARRAYS" << endl
         << "#             -------------------  -------------------  ---------------------" << endl
         << "#    Size     QuickSort  HeapSort  QuickSort  Heapsort  QuickSort   HeapSort" << endl
         << "#============================================================================" << endl;

    for (int exp = 15; exp <= 20; exp++) {
        size_t size = size_t(pow(2, exp));
        double total_time_quick = 0; // Tiempo total de QuickSort en pasos
        double total_time_heap = 0; // Tiempo total de HeapSort en pasos

        double total_time_quickSorted = 0; // Tiempo total de QuickSort en pasos
        double total_time_heapSorted = 0; // Tiempo total de HeapSort en pasos

        double total_time_quickReverseSorted=0;
        double total_time_heapReverseSorted=0;


        for (int trial = 1; trial <= numSamples; trial++) {
            int* vq = new int[size];
            int* vh = new int[size];
            
            if (!vq || !vh) {
                cerr << "Error, not enough memory!" << endl;
                exit(EXIT_FAILURE);  
            }

            for (size_t j = 0; j < size; j++) //rellenamos el vector vq aleatoriamente
                vq[j] = rand(); 

            for (size_t j = 0; j < size; j++) //copiamos el vector aleatorio de vq a vh asi tienen los mismo elementos
                vh[j] = vq[j];

            //random
            //quick
            int left = 0;
            size_t right = size - 1;
            _STEPS_ = 0;
            middle_QuickSort(vq, left, right);
            double pasosVqRandom = static_cast<double>(_STEPS_) / 1000000.0; // Convertir pasos a millones de pasos
            total_time_quick += pasosVqRandom; // Sumar al tiempo total de QuickSort

            //heap
            _STEPS_ = 0;
            heapSort(vh, size);
            double pasosVhRandom = static_cast<double>(_STEPS_) / 1000000.0; // Convertir pasos a millones de pasos
            total_time_heap += pasosVhRandom; // Sumar al tiempo total de HeapSort



            //sorted
            //quick
            
            _STEPS_ = 0;
            middle_QuickSort(vq, left, right);
            double pasosVqSorted = static_cast<double>(_STEPS_) / 1000000.0; // Convertir pasos a millones de pasos
            total_time_quickSorted += pasosVqSorted; // Sumar al tiempo total de QuickSort



            //heap
            _STEPS_ = 0;
            heapSort(vh, size);
            double pasosVhSorted = static_cast<double>(_STEPS_) / 1000000.0; // Convertir pasos a millones de pasos
            total_time_heapSorted += pasosVhSorted; // Sumar al tiempo total de HeapSort




            //reverse sorted
            //darle la vuelta al vector
            reverseArray(vq,size);
            for (size_t j = 0; j < size; j++) //copiamos el vector aleatorio de vq a vh asi tienen los mismo elementos
                vh[j] = vq[j];
            //quick
             _STEPS_ = 0;
            middle_QuickSort(vq, left, right);
            double pasosVqReverseSorted = static_cast<double>(_STEPS_) / 1000000.0; // Convertir pasos a millones de pasos
            total_time_quickReverseSorted += pasosVqReverseSorted; // Sumar al tiempo total de QuickSort



            //heap
            _STEPS_ = 0;
            heapSort(vh, size);
            double pasosVhReverseSorted= static_cast<double>(_STEPS_) / 1000000.0; // Convertir pasos a millones de pasos
            total_time_heapReverseSorted += pasosVhReverseSorted; // Sumar al tiempo total de HeapSort




            





            delete[] vq;
            delete[] vh;
        }

        double average_time_quick = total_time_quick / numSamples; // Calcular el promedio de QuickSort
        double average_time_heap = total_time_heap / numSamples; // Calcular el promedio de HeapSort

        double average_time_quickSorted = total_time_quickSorted / numSamples; // Calcular el promedio de QuickSort
        double average_time_heapSorted = total_time_heapSorted / numSamples; // Calcular el promedio de HeapSort
        
        double average_time_quickReverseSorted = total_time_quickReverseSorted / numSamples; // Calcular el promedio de QuickSort
        double average_time_heapReverseSorted = total_time_heapReverseSorted / numSamples; // Calcular el promedio de HeapSort



        cout << setw(9) << right << size 
            << setw(11) << fixed << setprecision(3) << average_time_quick 
            << setw(11) << average_time_heap << setw(11) << average_time_quickSorted 
            << setw(11) << average_time_heapSorted << setw(11) << average_time_quickReverseSorted
            << setw(11)<< average_time_heapReverseSorted << endl;

        
    }

    return 0;
}
