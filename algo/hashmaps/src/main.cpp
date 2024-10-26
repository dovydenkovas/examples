#include <fstream>
#include <iostream>
#include <ostream>
#include "hashmap.h"
#include <random>
#include <stdexcept>

using namespace std;


/**
 * Создать случайный ключ
 */
Key generate_key() {
    char l1 = rand()%26 + 'A';
    char l2 = rand()%26 + 'A';
    char l3 = rand()%26 + 'A';
    char n1 = rand()%10 + '0';
    char n2 = rand()%10 + '0';
    char n3 = rand()%10 + '0';
    return Key{{l1, n1, n2, n3, l2, l3}};
}

/**
 * Создать очень много слуайных ключей с
 * двойным хешированием, посчитать количество
 * попаданий в каждую ячейку и записать в файл.
 */
void test_hash() {
    cout << hashmap_capacity << endl;
    int *arr = new int[hashmap_capacity];
    for  (int i=0; i<10*hashmap_capacity; i++) {
        auto key = generate_key();
        for (int j=0; j<hashmap_capacity/10; ++j)
            arr[key.hash(j)%hashmap_capacity]++;
    }

    ofstream ofile("hash.csv");
    for  (int i=0; i<hashmap_capacity; i++)
        ofile << i << "; " << arr[i] << endl;

    delete[] arr;
}


/**
 * Создать много слуайных ключей без
 * двойного хеширования, посчитать количество
 * попаданий в каждую ячейку и записать в файл.
 */
void test_hash2() {
    cout << hashmap_capacity << endl;
    int *arr = new int[hashmap_capacity];
    for  (int i=0; i<hashmap_capacity; i++) {
        auto key = generate_key();
        arr[key.hash()%hashmap_capacity]++;
    }

    ofstream ofile("hash.csv");
    for  (int i=0; i<hashmap_capacity; i++)
        ofile << i << "; " << arr[i] << endl;

    delete[] arr;
}


int main() {
    srand(time(0));
    try {
        HashMap map;
        for (int i=0; i<15; ++i) {
            Key k = generate_key();
            map.insert(k);
            map.print();
        }
    } catch (Key::InvalidFormat &e) {
        cout << e.what() << endl;
    } catch (runtime_error &e) {
        cout << e.what() << endl;
    }

}
