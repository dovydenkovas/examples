#pragma once

enum Color: char {
    RED = 'r',
    BLACK = 'b'
};

template <typename K, typename T>
struct Node {
    K key;
    T data;
    Color color;

    Node(K k, T d, char c): key{k}, data{d}, color{c} {}
    Node(K k, T d): key{k}, data{d}, color{BLACK} {}
    Node(): key{}, data{}, color{} {}

    bool operator!=(const Node<K, T> &two) {
        return !(*this == two);
    }

    bool operator==(const Node<K, T> &two) {
        return this->key == two.key;
    }
};
