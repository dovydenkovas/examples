#pragma once
#include <iostream>
#include <ostream>
#include <vector>
#include <math.h>
#include "node.hpp"


template <class K = std::string, class T = std::string>
struct _Node {
        K key = {};
        T data = {};
        _Node<K, T>* parent = nullptr;
        _Node<K, T>* left = nullptr;
        _Node<K, T>* right = nullptr;
        Color color = BLACK; // for red-black trees

        _Node (K key, T data) : key(key), data(data) {}
        _Node () { nil(); }

        void nil() {
            is_nill_node = true;
            data = {};
            key = {};
            left = nullptr;
            right = nullptr;
        };

        void some() {
            is_nill_node = false;
        }

        bool is_nil() { return is_nill_node; }


private:
    bool is_nill_node = false;
};


template <class K = std::string, class T = std::string>
class BinTree {
public:
    BinTree();
    ~BinTree();
    void insert(K key, T data);
    void remove(K key);

    T* operator[](K key) const;
    T* search(K key) const;

    Node<K, T*> minimum() const;
    Node<K, T*> maximum() const;

    // Следующий по величине ключа элемент
    Node<K, T*> succeser(K key) const;

    // Предыдущий по величине ключа элемент
    Node<K, T*> predecessor(K key) const;

    // Симметричный обход
    void print() const;

    std::vector<Node<K, T>> get_static() const;

    int height() const;

protected:
    _Node<K, T>* root;

    // Функции, аналогичны верхним, но работают не со значениями,
    // а непосредственно с узлами дерева. Делать их публичными нельзя,
    // так как это нарушит инвариант бинарного дерева поиска.
    _Node<K, T>* search_node(K key) const;
    _Node<K, T>* minimum_node(_Node<K, T>* node) const;
    _Node<K, T>* maximum_node(_Node<K, T>* node) const;
    // Симметричный обход начиная с заданного узла
    void print_from_node(std::ostream &os, _Node<K, T>* node) const;
    void get_static(std::vector<Node<K, T>> &arr, _Node<K, T>* node, int deep, int id) const;

    // Поместить поддерево с корнем элемент вместо элемента position
    void transplant(_Node<K, T>* position, _Node<K, T>* element);


    void recursive_delete(_Node<K, T>* node);
    int height(_Node<K, T> *node) const;
};


template <class K, class T>
BinTree<K, T>::BinTree() {
    root = new _Node<K, T>;
    root->parent = new _Node<K, T>;
}


template <class K, class T>
void BinTree<K, T>::insert(K key, T data) {
    _Node<K, T> *node = search_node(key);
    node->some();
    node->key = key;
    node->data = data;
    if (node->left == nullptr)
        node->left = new _Node<K, T>;
    if (node->right == nullptr)
        node->right = new _Node<K, T>;
    node->left->parent = node;
    node->right->parent = node;
}


template <class K, class T>
void BinTree<K, T>::remove(K key) {
    _Node<K, T> *node = search_node(key);
    if (node->is_nil())
        return;

    if (node->left->is_nil()) {
        delete node->left; // nil
        transplant(node, node->right);
    } else if (node->right->is_nil()) {
        delete node->right; // nil
        transplant(node, node->left);
    }
    else {
        auto min = minimum_node(node->right);
        delete min->left; // nil

        if (min->parent != node) {
            transplant(min, min->right);
            min->right = node->right;
            min->right->parent = min;
        }

        transplant(node, min);
        min->left = node->left;
        min->left->parent = min;
    }

    if (node != root)
        delete node;
    else
        node->nil();
}


template <class K, class T>
T* BinTree<K, T>::search(K key) const {
    // Вот бы всегда код был таким тривиальным
    auto node = search_node(key);
    if (node->is_nil())
        return nullptr;
    return &node->data;
}


template <class K, class T>
Node<K, T*> BinTree<K, T>::minimum() const {
    // Минимальный элемент дерева
    auto min = minimum_node(root);
    if (min->is_nil())
            return {{}, nullptr};
    return {min->key, &min->data};
}

template <class K, class T>
_Node<K, T>* BinTree<K, T>::minimum_node(_Node<K, T>* node) const {
    // Минимальным элементом поддерева всегда является
    // самый левый нижний элемент.
    while (!node->is_nil() && !node->left->is_nil())
        node = node->left;
    return node;
}


template <class K, class T>
Node<K, T*> BinTree<K, T>::maximum() const {
    // Максимальный элемент дерева
    auto max = maximum_node(root);
    if (max->is_nil())
        return {K{}, nullptr};
    return {max->key, &max->data};
}

template <class K, class T>
_Node<K, T>* BinTree<K, T>::maximum_node(_Node<K, T> *node) const {
    // Максимальным элементом поддерева всегда является
    // самый правый нижний элемент.
    while (!node->is_nil() && !node->right->is_nil())
        node = node->right;
    return node;
}


// Следующий по величине ключа элемент
template <class K, class T>
Node<K, T*>  BinTree<K, T>::succeser(K key) const {
    _Node<K, T>* node = search_node(key);
    if (node->is_nil())
        return {K{}, nullptr};

    // Если у узла есть правое поддерево, то следующим элементом
    // будет минимальный элемент правого поддерева
    if (!node->right->is_nil()) {
        auto suc = minimum_node(node->right);
        if (suc->is_nil())
            return {K{}, nullptr};
        return {suc->key, &suc->data};
    }

    // В противном случае следующим по величине элементом является тот
    //  предок, для которого ветвь исходного элемента является левой
    while (!node->parent->is_nil()) {
        if (node->parent->left == node)
            return {node->parent->key, &node->parent->data};
        node = node->parent;
    }

    return {K{}, nullptr};
}

// Предыдущий по величине ключа элемент
template <class K, class T>
Node<K, T*> BinTree<K, T>::predecessor(K key) const {
    _Node<K, T>* node = search_node(key);
    if (node->is_nil())
        return {K{}, nullptr};

    // Если у узла есть левое поддерево, то предыдущим элементом
    // будет максимальный элемент левого поддерева
    if (!node->left->is_nil()) {

        auto pred = maximum_node(node->left);
        if (pred->is_nil())
            return {K{}, nullptr};
        return {pred->key, &pred->data};
    }

        // В противном случае предыдущим по величине элементом является тот
        //  предок, для которого ветвь исходного элемента является правой
        while (!node->parent->is_nil() && node->parent != nullptr) {
             std::cout << node->key << std::endl;
            if (node->parent->right == node)
                return {node->parent->key, &node->parent->data};
            node = node->parent;
        }

        return {K{}, nullptr};
}




template <class K, class T>
void BinTree<K, T>::recursive_delete(_Node<K, T>* node) {
    if (node == nullptr)
        return;
    recursive_delete(node->left);
    recursive_delete(node->right);
    delete node;
}


// Ищет узел, необходима для других функций
template <class K, class T>
_Node<K, T>* BinTree<K, T>::search_node(K key) const {
    // Поиск по двоичному дереву
    auto node = root;
    while (!node->is_nil() && node->key != key) {
        if (key < node->key)
            node = node->left;
        else
            node = node->right;
    }

    return node;
}


// Поместить поддерево с корнем элемент вместо элемента position
// Функция НЕ занимается удалением поддерева position.
template <class K, class T>
void BinTree<K, T>::transplant(_Node<K, T>* position, _Node<K, T>* element) {
    if (position->parent->is_nil())
        root = element;
    else if (position->parent->left == position)
        position->parent->left = element;
    else
        position->parent->right = element;

    element->parent = position->parent;
}

template <class K, class T>
BinTree<K, T>::~BinTree() {
    delete root->parent;
    recursive_delete(root);
}


// Симметричный обход
template <class K, class T>
void BinTree<K, T>::print() const {
    print_from_node(std::cout, root);
    std::cout << std::endl;
}


template <class K, class T>
void BinTree<K, T>::print_from_node(std::ostream &os, _Node<K, T>* node) const {
    // Симметричный обход массива
    if (node->is_nil())
        return;

    print_from_node(os, node->left);
    os << " (" << node->key << ", " << node->data << ") ";
    print_from_node(os, node->right);
}


template <class K, class T>
T* BinTree<K, T>::operator[](K key) const {
    return search(key);
}

template <class K, class T>
std::vector<Node<K, T>> BinTree<K, T>::get_static() const {
    std::vector<Node<K, T>> result;
    get_static(result, root, 1, 0);
    return result;
}

template <class K, class T>
void BinTree<K,T>::get_static(std::vector<Node<K, T>> &arr, _Node<K, T>* node, int deep, int id) const {
    if (!node->is_nil()) {
        int goal_size =  pow(2, deep) - 1;
        if (arr.size() < goal_size) {
            int size = arr.size();
            arr.resize(goal_size);
            for (int i=size; i<goal_size; ++i)
                arr[i] = {};
        }
        arr[id] = {node->key, node->data, node->color};
        get_static(arr, node->left, deep+1, 2*id+1);
        get_static(arr, node->right, deep+1, 2*id+2);
    }
}

template <class K, class T>
int BinTree<K, T>::height() const {
   return height(root);
}

template <class K, class T>
int BinTree<K, T>::height(_Node<K, T> *node) const {
    if (node->is_nil())
        return 0;
    return 1 + std::max(height(node->left), height(node->right));
}
