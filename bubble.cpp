#include <iostream>
#include <chrono>
#include <omp.h>
#include <cstdlib>
#include <ctime>
 
using namespace std;
using namespace std::chrono;
 
// Sequential Bubble Sort
void bubbleSort(int arr[], int n) {
    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < n - i - 1; ++j) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}
 
// Parallel Bubble Sort
void parallelBubbleSort(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        int start = i % 2;
#pragma omp parallel for
        for (int j = start; j < n - 1; j += 2) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

// Print array
void printArray(int arr[], int size) {
    for (int i = 0; i < size; ++i)
        cout << arr[i] << " ";
    cout << endl;
}
 
// Random array generation
void generateRandomArray(int arr[], int n) {
    for (int i = 0; i < n; ++i) {
        arr[i] = rand() % 1000;  // Random numbers between 0 and 999
    }
}
 
int main() {
    int n;
    cout << "Enter the size of the array: ";
    cin >> n;
 
    srand(time(0));  // Seed for random number generation
 
    int* arr = new int[n];
    int* original = new int[n];
 
    generateRandomArray(arr, n);
    for (int i = 0; i < n; ++i) {
        original[i] = arr[i];
    }
 
    cout << "\nOriginal array: ";
    printArray(arr, n);
 
    high_resolution_clock::time_point start, end;
    duration<double, milli> duration;
    
    // Parallel Bubble Sort
    for (int i = 0; i < n; ++i)
        arr[i] = original[i];
 
    start = high_resolution_clock::now();
    parallelBubbleSort(arr, n);
    end = high_resolution_clock::now();
    duration = end - start;
    cout << "\nParallel Bubble Sorted array: ";
    printArray(arr, n);
    cout << "Time taken: " << duration.count() << " milliseconds\n";
 
    // Sequential Bubble Sort
    for (int i = 0; i < n; ++i)
        arr[i] = original[i];
 
    start = high_resolution_clock::now();
    bubbleSort(arr, n);
    end = high_resolution_clock::now();
    duration = end - start;
    cout << "\nSequential Bubble Sorted array: ";
    printArray(arr, n);
    cout << "Time taken: " << duration.count() << " milliseconds\n";
 
 
    
    // Print the number of threads used in parallel sections
    int num_threads;
#pragma omp parallel
    {
#pragma omp single
        {
            num_threads = omp_get_num_threads();
        }
    }
    cout << "\nNumber of threads used: " << num_threads << endl;
 
    delete[] arr;
    delete[] original;
 
    return 0;
}
