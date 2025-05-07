#include <iostream>
#include <chrono>
#include <omp.h>
#include <cstdlib>
#include <ctime>
 
using namespace std;
using namespace std::chrono;
 
 
// Merge function for Merge Sort
void merge(int arr[], int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;
 
    int* L = new int[n1];
    int* R = new int[n2];
 
    for (int i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];
 
    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        arr[k++] = (L[i] <= R[j]) ? L[i++] : R[j++];
    }
 
    while (i < n1)
        arr[k++] = L[i++];
    while (j < n2)
        arr[k++] = R[j++];
 
    delete[] L;
    delete[] R;
}
 
// Sequential Merge Sort
void mergeSort(int arr[], int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}
 
// Parallel Merge Sort
void parallelMergeSort(int arr[], int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
#pragma omp parallel sections
        {
#pragma omp section
            parallelMergeSort(arr, l, m);
 
#pragma omp section
            parallelMergeSort(arr, m + 1, r);
        }
        merge(arr, l, m, r);
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
 
    // Sequential Merge Sort
    for (int i = 0; i < n; ++i)
        arr[i] = original[i];
 
    start = high_resolution_clock::now();
    mergeSort(arr, 0, n - 1);
    end = high_resolution_clock::now();
    duration = end - start;
    cout << "\nSequential Merge Sorted array: ";
    printArray(arr, n);
    cout << "Time taken: " << duration.count() << " milliseconds\n";
 
    // Parallel Merge Sort
    for (int i = 0; i < n; ++i)
        arr[i] = original[i];
 
    start = high_resolution_clock::now();
#pragma omp parallel
    {
#pragma omp single
        {
            parallelMergeSort(arr, 0, n - 1);
        }
    }
    end = high_resolution_clock::now();
    duration = end - start;
    cout << "\nParallel Merge Sorted array: ";
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
