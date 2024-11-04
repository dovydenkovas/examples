#include <cmath>
#include <iostream>
#include <vector>
#include "array_control.hpp"


using namespace std;

inline int minrun(int n) {
    int r = 0;
    while (n >= 64) {
        r |= (n & 1);
        n >>= 1;
    }
    return n + r;
}


template <typename T>
void isort(vector<T> &arr) {
    for (int i=1; i<arr.size(); ++i) {
        int j = i;
        T buff = arr[i];
        while (j > 0 && buff < arr[j-1]) {
            arr[j] = arr[j-1];
            --j;
        }
        arr[j] = buff;
    }
}

template <typename T>
void timsort(vector<T> &arr) {
    if (arr.size() <= 1)
        return;

    int mr = minrun(arr.size());


    int left_size = ceil(arr.size()/2.0);
    int right_size = arr.size() / 2;
    vector<T> left(left_size);
    vector<T> right(right_size);

    for (int i = 0; i < left.size(); ++i)
        left[i] = arr[i];

    for (int j = 0; j < right.size(); ++j)
        right[j] = arr[j+left_size];

    timsort(left);
    timsort(right);

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
    //timsort(arr);
    for (int i=0; i<100; i+= 1) {
        cout << i << " " << minrun(i) << endl;
    }
    print_array(arr);
}
