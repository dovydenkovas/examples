#pragma once
#include <algorithm>
#include <cctype>
#include <climits>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <exception>
#include <math.h>
#include <string>

using namespace std;

/**
 * Структура данных хеш-таблица.
 * Используется закрытое двойное хеширование.
 *
 */

 // Поиск произведения простых делителей числа
int get_base(int a);


// Исходный размер хеш-таблицы
// В данной реализации
constexpr int hashmap_capacity = 10;


// Максимальный размер хеш-ключа
// должен быть не меньше ожидаемого количества элементов таблицы
// и кратен hashmap_capacity и 2
constexpr int max_hash = hashmap_capacity*4;


// Смещение разрядов при вычислении hash1.
const int min_hash1_minor_digit_position = (max(0.0, log2(pow( max_hash / 26, 2))) + max(0.0, log2(pow(max_hash, 2)))) / 2;

// Константа для вычисления hash2,
// Нужна для того, чтобы при поиске ключа в таблицы
// посещались все ячейки.
// Технически является произведением простых делителей max_hash.
const int base_k = get_base(max_hash);

/**
 * Ключ формата "БцццББ"
 */
class Key {
public:
    class InvalidFormat: public exception {
            string message;
        public:
            InvalidFormat(string m): message{m}  {}
            string what() {return message;}
    };
    Key(string key) {set(key);}
    Key(): key{"000000"} {}

    void set(string key);
    string str() {return key;}
    bool operator==(const Key &key) {return this->key == key.key;}
    bool operator!=(const Key &key) {return !(*this == key); }
    uint16_t hash(int iteration=0);

    bool is_nil() { return key == "000000"; }

    static Key removed() {
        Key removed;
        removed.key = "rrrrrr";
        return removed;
    }


private:
    string key;

    uint16_t hash1();
    uint16_t hash2();
};


class HashMap {
public:
    ~HashMap();
    HashMap();
    void insert(Key key);
    void remove(Key key);

    Key operator[](size_t index);
    bool search(Key key);

    // Симметричный обход
    void print() const;

private:
    Key *array = nullptr;
    int *statistic = nullptr;
    size_t array_size;
    size_t map_size;

    void resize(size_t new_size);
};
