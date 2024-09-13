#include <algorithm>
#include <cctype>
#include <iostream>
#include <cstdlib>
#include <chrono> // For high-resolution clock

#define SMALL 10
#define MEDIUM 100
#define LARGE 1000
#define TESTAMT 10

bool checkSort(int array[], int size) {
    for(int i = 0; i < size - 1; i++) {
        if(array[i] > array[i + 1])
            return false;
    }
    return true;
}

void LinearSort(int array[], int size) {
    bool isSorted = checkSort(array, size);
    if(isSorted) return;

    for(int i = 0; i < size - 1; i++) {
        if(array[i] > array[i + 1]) {
            int temp = array[i];
            array[i] = array[i + 1];
            array[i + 1] = temp;
            LinearSort(array, size); // Sort recursively
        }
    }
}

int* generateArray(int amount) {
    int* array = (int*)malloc(amount * sizeof(int));
    if (array == nullptr) {
        std::cerr << "Memory allocation failed!" << std::endl;
        exit(1);
    }
    for(int i = 0; i < amount; i++) {
        array[i] = rand() % 1000; // Generate random numbers
    }
    return array;
}

void Merge(int* array, int left, int mid, int right) {
    int i, j, k;
    int n1 = mid - left + 1;
    int n2 = right - mid;

    int* L = (int*)malloc(n1 * sizeof(int));
    int* R = (int*)malloc(n2 * sizeof(int));
    if (L == nullptr || R == nullptr) {
        std::cerr << "Memory allocation failed!" << std::endl;
        exit(1);
    }

    for (i = 0; i < n1; i++)
        L[i] = array[left + i];
    for (j = 0; j < n2; j++)
        R[j] = array[mid + 1 + j];

    i = 0; // Initial index of first subarray
    j = 0; // Initial index of second subarray
    k = left; // Initial index of merged subarray

    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            array[k] = L[i];
            i++;
        } else {
            array[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        array[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        array[k] = R[j];
        j++;
        k++;
    }

    free(L);
    free(R);
}

void MergeSort(int array[], int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        MergeSort(array, left, mid);
        MergeSort(array, mid + 1, right);

        Merge(array, left, mid, right);
    }
}

double average(double* array, int size) {
    double result = 0.0;
    for(int i = 0; i < size; i++) {
        result += array[i];
    }
    return result / size;
}

int main() {
    srand(static_cast<unsigned>(time(0))); // Random seed every time.
    double* LinearTime = (double*)malloc(TESTAMT * sizeof(double));
    double* MergeTime = (double*)malloc(TESTAMT * sizeof(double));
    if (LinearTime == nullptr || MergeTime == nullptr) {
        std::cerr << "Memory allocation failed!" << std::endl;
        return 1;
    }

    std::cout << "What size do you want to test? S, M, L" << std::endl; // Determines test
    char response;
    std::cin >> response;
    int testSize;

    switch (std::toupper(response)) {
        case 'S':
            testSize = SMALL;
            break;
        case 'M':
            testSize = MEDIUM;
            break;
        case 'L':
            testSize = LARGE;
            break;
        default:
            std::cout << "Invalid option. Please choose S, M, or L." << std::endl;
            free(LinearTime);
            free(MergeTime);
            return 0;
    }

    // Linear Testing
    for(int i = 0; i < TESTAMT; i++) {
        int* Array = generateArray(testSize);
        auto start = std::chrono::high_resolution_clock::now();
        LinearSort(Array, testSize);
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> duration = end - start;
        LinearTime[i] = duration.count();
        free(Array);
    }

    // Merge Testing
    for(int i = 0; i < TESTAMT; i++) {
        int* Array = generateArray(testSize);
        auto start = std::chrono::high_resolution_clock::now();
        MergeSort(Array, 0, testSize - 1);
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> duration = end - start;
        MergeTime[i] = duration.count();
        free(Array);
    }

    std::sort(LinearTime, LinearTime + TESTAMT);
    std::sort(MergeTime, MergeTime + TESTAMT);

    std::cout << "Linear Time stuff\n" << std::endl;
    std::cout << "Best : " << LinearTime[0] << " ms" << std::endl;
    std::cout << "Average : " << average(LinearTime, TESTAMT) << " ms" << std::endl;
    std::cout << "Worst : " << LinearTime[TESTAMT - 1] << " ms" << std::endl;

    std::cout << "\n\nMerge Time stuff\n";
    std::cout << "Best : " << MergeTime[0] << " ms" << std::endl;
    std::cout << "Average : " << average(MergeTime, TESTAMT) << " ms" << std::endl;
    std::cout << "Worst : " << MergeTime[TESTAMT - 1] << " ms" << std::endl;

    free(LinearTime);
    free(MergeTime);
    return 0;
}
