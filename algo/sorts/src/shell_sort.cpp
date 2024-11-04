#include <iostream>
#include <vector>
#include "array_control.hpp"


using namespace std;


template <typename T>
void shsort(vector<T> &arr) {
    for (int d = arr.size() / 2; d != 0; d /= 2)
        for (int i = d; i < arr.size(); ++i)
            for (int j = i; j >= d && arr[j] < arr[j-d]; j -= d)
                swap(arr[j], arr[j-d]);
}


int main() {
    vector<int> arr = {9, 8, 7, 6, 4, 5, 1, 2, 3};

    print_array(arr);
    shsort(arr);
    print_array(arr);
}
