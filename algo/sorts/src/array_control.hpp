#pragma once
#include <vector>
#include <iostream>


void read_array(std::vector<int> &array) {
    while (!std::cin.eof()) {
        int buff;
        std::cin >> buff;
        if (!std::cin)
            break;
        array.push_back(buff);
    }
}


void print_array(std::vector<int> array) {
    std::cout << "[";
    for (int i = 0; i < array.size(); ++i) {
        if (array.size() > 100 && i % 10 == 0) std::cout << std::endl << "\t";
        std::cout << array[i];
        if (i + 1 < array.size()) std::cout << ", ";
    }
    if (array.size() > 100) std::cout << "\n]\n";
    else std::cout << "]\n";
}


void print_subarray(std::vector<int> array, int left, int right) {
    std::cout << "[ ... ";
    for (int i = left; i < right; ++i) {
        if (array.size() > 100 && i % 10 == 0) std::cout << std::endl << "\t";
        std::cout << array[i];
        if (i + 1 < array.size()) std::cout << ", ";
    }
    if (array.size() > 100) std::cout << "\n]\n";
    else std::cout << " ... ]\n";
}
