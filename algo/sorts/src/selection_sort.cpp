#include <iostream>
#include <vector>
#include "array_control.hpp"


using namespace std;


template <typename T>
void ssort(vector<T> &arr) {
    for (int i=0; i<arr.size(); ++i)
        for (int j=i; j<arr.size(); ++j)
            if (arr[i] > arr[j])
                swap(arr[i], arr[j]);
}


int main() {
    vector<int> arr = {9, 8, 7, 6, 4, 5, 1, 2, 3};

    print_array(arr);
    ssort(arr);
    print_array(arr);
}
