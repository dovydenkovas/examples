#include <vector>
#include "array_control.hpp"

using namespace std;

void sorting_tree(vector<int> &arr, int size, int node=0) {
    int left  = 2*node + 1;
    int right = 2*node + 2;
    if (left < size) {
        sorting_tree(arr, size, left);
        if (arr[left] > arr[node])
            swap(arr[left], arr[node]);
    }

    if (right < size) {
        sorting_tree(arr, size, right);
        if (arr[right] > arr[node])
            swap(arr[node], arr[right]);
    }
}

void heap_sort(vector<int> &arr) {
    int heap_size = arr.size();

    for (int i=0; i<arr.size(); i++) {
        sorting_tree(arr, heap_size-i);
        swap(arr[0], arr[heap_size-i-1]);
    }
}

#ifndef MAKE_TESTS
int main() {
    vector<int> arr =  {9, 8, 12, 7, 6, 12, 4, 5, 1, 2, 3, 12};

    print_array(arr);
    heap_sort(arr);
    print_array(arr);
}
#endif
