#include <iostream>
#include <vector>
#include "array_control.hpp"


using namespace std;

constexpr int AMOUNT_COUNT = 20;

void dsort(vector<int> &arr) {
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


int main() {
    vector<int> arr = {9, 8, 12, 7, 6, 12, 4, 5, 1, 2, 3, 12};

    print_array(arr);
    dsort(arr);
    print_array(arr);
}
