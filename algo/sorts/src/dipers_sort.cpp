#include <iostream>
#include <vector>
#include "array_control.hpp"


using namespace std;

constexpr int AMOUNT_COUNT = 10000;

void dipers_sort(vector<int> &arr) {
    // Временная сложность O(m + n)
    // Пространственная O(m+n)
    vector<int> amount(AMOUNT_COUNT, 0);
    for (int i=0; i<arr.size(); i++)
        amount[arr[i]]++;

    int k = 0;
    for (int i=0; i<amount.size(); i++) {
        while (amount[i] > 0) {
            arr[k] = i;
            ++k;
            --amount[i];
        }
    }
}


#ifndef MAKE_TESTS
int main() {
    vector<int> arr = {9, 8, 12, 7, 6, 12, 4, 5, 1, 2, 3, 12};

    print_array(arr);
    dipers_sort(arr);
    print_array(arr);
}
#endif
