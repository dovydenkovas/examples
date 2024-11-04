#include <iostream>
#include <vector>
#include "array_control.hpp"


using namespace std;


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


int main() {
    vector<int> arr = {9, 8, 7, 6, 4, 5, 1, 2, 3};

    print_array(arr);
    isort(arr);
    print_array(arr);
}
