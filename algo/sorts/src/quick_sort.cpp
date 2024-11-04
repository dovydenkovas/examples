#include <iostream>
#include <vector>
#include "array_control.hpp"


using namespace std;


template <typename T>
void qsort(vector<T> &arr) {
    if (arr.size() <= 1)
        return;

    vector<T> less;
    vector<T> more;
    T border = (arr[0] + arr[arr.size()/2] + arr[arr.size()-1])/3;
    for (int i = 0; i < arr.size(); ++i)
        if (arr[i] <= border)
            less.push_back(arr[i]);
        else
            more.push_back(arr[i]);

    qsort(less);
    qsort(more);

    int k = 0;
    for (int i = 0; i < less.size(); ++i) {
        arr[k] = less[i];
        ++k;
    }

    for (int i = 0; i < more.size(); ++i) {
        arr[k] = more[i];
        ++k;
    }
}


int main() {
    vector<int> arr = {9, 8, 7, 6, 4, 5, 1, 2, 3};

    print_array(arr);
    qsort(arr);
    print_array(arr);
}
