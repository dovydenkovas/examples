#include <cmath>
#include <iostream>
#include <vector>
#include "array_control.hpp"


using namespace std;


template <typename T>
void msort(vector<T> &arr) {
    if (arr.size() <= 1)
        return;

    int left_size = ceil(arr.size()/2.0);
    int right_size = arr.size() / 2;
    vector<T> left(left_size);
    vector<T> right(right_size);

    for (int i = 0; i < left.size(); ++i)
        left[i] = arr[i];

    for (int j = 0; j < right.size(); ++j)
        right[j] = arr[j+left_size];

    msort(left);
    msort(right);

    int i = 0;
    int j = 0;
    int k = 0;

    while (i < left_size && j < right_size) {
        if (left[i] < right[j]) {
            arr[k] = left[i];
            ++i;
        } else {
            arr[k] = right[j];
            ++j;
        }
        ++k;
    }

    while (i < left_size) {
        arr[k] = left[i];
        ++i;
        ++k;
    }

    while (j < right_size) {
        arr[k] = right[j];
        ++j;
        ++k;
    }
}


int main() {
    vector<int> arr = {9, 8, 7, 6, 4, 5, 1, 2, 3};

    print_array(arr);
    msort(arr);
    print_array(arr);
}
