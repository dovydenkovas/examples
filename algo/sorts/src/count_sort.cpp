#include <iostream>
#include <vector>
#include "array_control.hpp"


using namespace std;


template <typename T>
void csort(vector<T> &arr) {
    vector<T> old = arr;
    for (int i=0; i<old.size(); i++) {
        int c = 0;
        for (int j=0; j<old.size(); j++)
            if (old[i] > old[j] || old[i] == old[j] && i < j)
                c++;

        arr[c] = old[i];
    }
}


int main() {
    vector<int> arr = {9, 8, 7, 6, 4, 5, 1, 2, 3};

    print_array(arr);
    csort(arr);
    print_array(arr);
}
