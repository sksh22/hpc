#include <iostream>
#include <omp.h>
#include <vector>
using namespace std;

int main() {
    const int N = 10;
    vector<int> data(N);
    for (int i = 0; i < N; i++) data[i] = rand() % 100;

    int min_val = data[0], max_val = data[0], sum = 0;

    // Start parallel region and display threads
    #pragma omp parallel
    {
        #pragma omp single
        cout << "Threads used: " << omp_get_num_threads() << endl;
    }

    // Parallel reduction for min, max, and sum
    #pragma omp parallel for reduction(min:min_val) reduction(max:max_val) reduction(+:sum)
    for (int i = 0; i < N; i++) {
        if (data[i] < min_val) min_val = data[i];
        if (data[i] > max_val) max_val = data[i];
        sum += data[i];
    }

    cout << "Min: " << min_val << ", Max: " << max_val
         << ", Sum: " << sum << ", Avg: " << (float)sum / N << endl;
         
    for(int i:data){
        cout<<i<<" ";
    }
    return 0;
}
