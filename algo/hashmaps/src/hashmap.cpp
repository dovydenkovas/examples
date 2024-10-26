#include <cctype>
#include <climits>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <math.h>
#include <stdexcept>
#include <string>
#include <iostream>
#include "hashmap.h"

using namespace std;

/**
 * Структура данных хеш-таблица.
 * Используется закрытое двойное хеширование.
 *
 */

 // Поиск произведения простых делителей числа
 int get_base(int a) {
     int base = 1;
     int k = 2;
     while (a > 1) {
         if (a % k == 0) {
             base *= k;
             while (a % k == 0)
                a /= k;
         }
         k += 1;
     }

     return base;
 }


void Key::set(string k) {
    if (k.length() != 6)
        throw InvalidFormat("Неправильная длина ключа.");
    if (isdigit(k[1]) && isdigit(k[2]) && isdigit(k[3]) && // Проверка позиций цифр.
        isalpha(k[0]) && isupper(k[0]) && // Проверка 1-й буквы
        isalpha(k[4]) && isupper(k[4]) && // 2-й буквы
        isalpha(k[5]) && isupper(k[5]))   // 3-й буквы
            key = k;
    else throw InvalidFormat("Нарушен формат ключа.");
}

uint16_t Key::hash(int iteration) {
    return (hash1() + iteration * hash2()) % max_hash;
}

uint16_t Key::hash1() {
    // Метод умножения
    // double k = 0;

    // k += pow((key[0] - 'A') * max_hash / 26.0, 2);
    // k += pow((key[1] - '0') * max_hash / 10.0, 2);
    // k += pow((key[2] - '0') * max_hash / 10.0, 2);
    // k += pow((key[3] - '0') * max_hash / 10.0, 2);
    // k += pow((key[4] - 'A') * max_hash / 26.0, 2);
    // k += pow((key[5] - 'A') * max_hash / 26.0, 2);

    // return (uint64_t)round(k) >> 12;

    // Метод свертки, адаптированный для лабы)
    uint16_t a = int((key[0] - 'A') * 255 / 26) << 7;
    a = a ^ int((key[1] - '0' + 1.0) * 255 / 10);
    uint16_t b = int((key[2] - '0') * 255 / 10) << 7;
    b = b ^ int((key[3] - '0' + 1.0) * 255 / 10);
    uint16_t c = int((key[4] - 'A') * 255 / 26) << 7;
    c = c ^ int((key[5] - 'A') * 255 / 26);

    return a^b^c;
}


/**
 * Метод умножения, адаптированный под лабораторку.
 */
uint16_t Key::hash2() {
    double k = 0;
    k += (key[0] - 'A' + 1.0) * max_hash / 26;
    k += (key[1] - '0' + 1.0) * max_hash / 10;
    k += (key[2] - '0' + 1.0) * max_hash / 10;
    k += (key[3] - '0' + 1.0) * max_hash / 10;
    k += (key[4] - 'A' + 1.0) * max_hash / 26;
    k += (key[5] - 'A' + 1.0) * max_hash / 26;
    return base_k*(int)k+1;
}

// Метод умножения
uint16_t hash3() {
    int max_hash  = 2500;
    string key = "I666MP";

    int k = 0;
    k += (key[0] - 'A') * max_hash / 26;
    k += (key[1] - '0') * max_hash / 10;
    k += (key[2] - '0') * max_hash / 10;
    k += (key[3] - '0') * max_hash / 10;
    k += (key[4] - 'A') * max_hash / 26;
    k += (key[5] - 'A') * max_hash / 26;

    double A = 0.6180339887;
    return max_hash * (k*A - floor(k*A));
}

HashMap::HashMap() {
    resize(hashmap_capacity);
}

HashMap::~HashMap() {
    if (array != nullptr)
        delete[] array;
}

void HashMap::resize(size_t new_size) {
    cout << "resize" << endl;
    Key *old_array = array;
    size_t old_size = array_size;
    map_size = 0;
    array_size = new_size;
    array = new Key[array_size];

    if (old_array == nullptr)
        return;

    for (int i=0; i<old_size; i++)
        if (!old_array[i].is_nil())
            insert(old_array[i]);

    delete[] old_array;
}

void HashMap::insert(Key k) {
    if (0.9*array_size < map_size)
        resize(array_size * 2);
    cout << k.str() << endl;
    uint16_t h = k.hash();
    int i = 0;
    Key removed = Key::removed();
    while (!array[h%array_size].is_nil() && array[h%array_size] != removed && i <= array_size) {
        h = k.hash(i);
        cout << h % array_size << endl;
        ++i;
    }

    if (i > array_size)
       throw runtime_error("Error put value");

    array[h%array_size] = k;
    ++map_size;
}

void HashMap::remove(Key k) {
    uint16_t h = k.hash();
    int i = 0;
    while (!array[h%array_size].is_nil() && i < array_size) {
        h = k.hash(i);
        ++i;
    }

    if (i >= array_size)
       throw runtime_error("Error remove value");

    array[h%array_size] = Key::removed();
    --map_size;
}

bool HashMap::search(Key k) {
    uint16_t h = k.hash();
    int i = 0;
    while (!array[h%array_size].is_nil() && i < array_size) {
        h = k.hash(i);
        ++i;
    }

    if (i >= array_size)
        throw runtime_error("Error search value");

    return array[h%array_size] == Key::removed();
}

void HashMap::print() const {
    for (int i=0; i<array_size; ++i)
        if (array[i].is_nil())
            cout << "   nil ";
        else if (array[i] == Key::removed())
            cout << "rem ";
        else
            cout << array[i].str() << " ";
    cout << endl;
}
