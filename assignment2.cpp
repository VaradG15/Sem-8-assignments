#include <iostream>
#include <omp.h>
#include <vector>
#include <cstdlib>

using namespace std;

vector<int> deepCopy(vector<int> &arraySource) {
    vector<int> arrayDestination;
    for (int i = 0; i < arraySource.size(); i++) {
        arrayDestination.push_back(arraySource[i]);
    }
    return arrayDestination;
}

void bubbleSort(vector<int> &array) {
    int n = array.size();
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (array[j] > array[j + 1]) {
                swap(array[j], array[j + 1]);
            }
        }
    }
}

void parallelBubbleSort(vector<int> &array) {
    int n = array.size();
    for (int i = 0; i < n - 1; i++) {
#pragma omp parallel for
        for (int j = 0; j < n - i - 1; j++) {
            if (array[j] > array[j + 1]) {
#pragma omp critical
                {
                    swap(array[j], array[j + 1]);
                }
            }
        }
    }
}

void merge(vector<int> &arr, int start, int end, int mid) {
    vector<int> L(arr.begin() + start, arr.begin() + mid + 1);
    vector<int> R(arr.begin() + mid + 1, arr.begin() + end + 1);

    int i = 0, j = 0, k = start;
    while (i < L.size() && j < R.size()) {
        if (L[i] <= R[j]) {
            arr[k++] = L[i++];
        } else {
            arr[k++] = R[j++];
        }
    }
    while (i < L.size()) arr[k++] = L[i++];
    while (j < R.size()) arr[k++] = R[j++];
}

void mergeSort(vector<int> &array, int start, int end) {
    if (start < end) {
        int mid = (start + end) / 2;
        mergeSort(array, start, mid);
        mergeSort(array, mid + 1, end);
        merge(array, start, end, mid);
    }
}

void mergeSortParallel(vector<int> &array, int start, int end) {
    if (start < end) {
        int mid = (start + end) / 2;
#pragma omp parallel sections
        {
#pragma omp section
            mergeSortParallel(array, start, mid);
#pragma omp section
            mergeSortParallel(array, mid + 1, end);
        }
        merge(array, start, end, mid);
    }
}

int main() {
    vector<int> array;
    for (int i = 0; i < 50000; i++) {
        array.push_back(rand() % 100000);
    }

    vector<int> arr_cp1 = deepCopy(array);

    double start = omp_get_wtime();
    bubbleSort(array);
    double end = omp_get_wtime();
    cout << "Time for Sequential Bubble Sort: " << end - start << " seconds\n";

    start = omp_get_wtime();
    parallelBubbleSort(arr_cp1);
    end = omp_get_wtime();
    cout << "Time for Parallel Bubble Sort: " << end - start << " seconds\n";

    auto arr_cp2 = deepCopy(array);
    start = omp_get_wtime();
    mergeSort(arr_cp2, 0, arr_cp2.size() - 1);
    end = omp_get_wtime();
    cout << "Time for Sequential Merge Sort: " << end - start << " seconds\n";

    auto arr_cp3 = deepCopy(array);
    start = omp_get_wtime();
    mergeSortParallel(arr_cp3, 0, arr_cp3.size() - 1);
    end = omp_get_wtime();
    cout << "Time for Parallel Merge Sort: " << end - start << " seconds\n";

    return 0;
}
