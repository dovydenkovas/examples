#include <iostream>
#include <vector>
#include "array_control.hpp"


using namespace std;


template <typename T>
void quick_sort(vector<T> &arr) {
    if (arr.size() <= 1)
        return;

    vector<T> less;
    vector<T> more;
    vector<T> equal;
    T border = (arr[0] + arr[arr.size()/2] + arr[arr.size()-1])/3;
    for (int i = 0; i < arr.size(); ++i) {
        if (arr[i] < border)
            less.push_back(arr[i]);
        else if (arr[i] > border)
            more.push_back(arr[i]);
        else
            equal.push_back(arr[i]);
    }

    quick_sort(less);
    quick_sort(more);

    int k = 0;
    for (int i = 0; i < less.size(); ++i) {
        arr[k] = less[i];
        ++k;
    }

    for (int i = 0; i < equal.size(); ++i) {
        arr[k] = equal[i];
        ++k;
    }

    for (int i = 0; i < more.size(); ++i) {
        arr[k] = more[i];
        ++k;
    }
}


#ifndef MAKE_TESTS
int main() {
    vector<int> arr = {9, 8, 7, 6, 4, 5, 1, 2, 3};

    print_array(arr);
    quick_sort(arr);
    print_array(arr);
}
#endif
