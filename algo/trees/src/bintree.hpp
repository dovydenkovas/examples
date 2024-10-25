#pragma once
#include <iostream>
#include <ostream>
#include <utility>
#include <vector>
#include <math.h>

using std::pair;

template <typename K, typename T>
struct Node {
    K key;
    T data;
    char color;

    Node(K k, T d, char c): key{k}, data{d}, color{c} {}
    Node(): key{}, data{}, color{} {}

    bool operator!=(const Node<K, T> &two) {
        return !(*this == two);
    }

    bool operator==(const Node<K, T> &two) {
        return this->key == two.key;
    }

};

template <typename K, typename T>
struct _Node {
        K key;
        T data;
        _Node<K, T>* parent = nullptr;
        _Node<K, T>* left = nullptr;
        _Node<K, T>* right = nullptr;
        char color = 'w'; // for red-black trees

        _Node (K key, T data, _Node<K, T> *left, _Node<K, T> *right) : key(key), data(data), left(left), right(right) {
                /*if (left)
                    left->parent = this;
                if (right)
                    right->parent = this;*/
            }

        _Node (K key, T data) : key(key), data(data) {}
};


template <typename K, typename T>
class BinTree {
public:
    BinTree();
    ~BinTree();
    void insert(K key, T data);
    void remove(K key);

    //std::ostream& operator<<(std::ostream &os) const;
    std::pair<K, T*> operator[](K key);

    T* search(K key);
    std::pair<K, T*> minimum() const;
    std::pair<K, T*> maximum() const;
    // Следующий по величине ключа элемент
    std::pair<K, T*> succeser(K key) const;
    // Предыдущий по величине ключа элемент
    std::pair<K, T*> predecessor(K key) const;
    // Симметричный обход
    void print() const;
    std::vector<Node<K, T>> get_static() const;

    int height() const;

protected:
    _Node<K, T> *nil;
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
    _Node<K, T>* root;

    void recursive_delete(_Node<K, T>* node);
    int height(_Node<K, T> *node) const;
};

template <typename K, typename T>
BinTree<K, T>::BinTree() {
    this->nil = new _Node<K, T>{{}, {}};
    this->root = this->nil;
}


template <typename K, typename T>
void BinTree<K, T>::insert(K key, T data) {
    _Node<K, T>* new_node = new _Node<K, T>{key, data, nil, nil};
    if (root == nil) {
        root = new_node;
        new_node->parent = nil;
        return;
    }
    _Node<K, T>* node = root;
    _Node<K, T>* parent = nil;
    while (node != nil) {
        parent = node;
        if (key < node->key)
            node = node->left;
        else
            node = node->right;
    }

    new_node->parent = parent;
    if (key < parent->key)
        parent->left = new_node;
    else
        parent->right = new_node;
}


template <typename K, typename T>
void BinTree<K, T>::remove(K key) {
    auto node = search_node(key);
    if (node == nil)
        return;

    if (node->left == nil)
        transplant(node, node->right);
    else if (node->right == nil)
        transplant(node, node->left);

    else {
        auto min = minimum_node(node->right);
        std::cout << min->key << std::endl;

        if (min->parent != node) {
            transplant(min, min->right);
            min->right = node->right;
            min->right->parent = min;
        }

        transplant(node, min);
        min->left = node->left;
        min->left->parent = min;
        //std::cout << node->right->key << std::endl;
    }

    delete node;
}


template <typename K, typename T>
T* BinTree<K, T>::search(K key) {
    // Вот бы всегда код был таким тривиальным
    auto node = search_node(key);
    if (node == nil)
        return nullptr;
    return &node->data;
}


template <typename K, typename T>
pair<K, T*> BinTree<K, T>::minimum() const {
    // Минимальный элемент дерева
    auto min = minimum_node(root);
    if (min == nil)
            return {K{}, nullptr};
    return {min->key, &min->data};
}

template <typename K, typename T>
_Node<K, T>* BinTree<K, T>::minimum_node(_Node<K, T>* node) const {
    // Минимальным элементом поддерева всегда является
    // самый левый нижний элемент.
    while (node != nil && node->left != nil)
        node = node->left;
    return node;
}


template <typename K, typename T>
pair<K, T*> BinTree<K, T>::maximum() const {
    // Максимальный элемент дерева
    auto max = maximum_node(root);
    if (max == nil)
        return {K{}, nullptr};
    return {max->key, &max->data};
}

template <typename K, typename T>
_Node<K, T>* BinTree<K, T>::maximum_node(_Node<K, T> *node) const {
    // Максимальным элементом поддерева всегда является
    // самый правый нижний элемент.
    while (node != nil && node->right != nil)
        node = node->right;
    return node;
}


// Следующий по величине ключа элемент
template <typename K, typename T>
pair<K, T*>  BinTree<K, T>::succeser(K key) const {
    _Node<K, T>* node = search_node(key);
    if (node == nil)
        return {K{}, nullptr};

    // Если у узла есть правое поддерево, то следующим элементом
    // будет минимальный элемент правого поддерева
    if (node->right != nil) {
        auto suc = minimum_node(node->right);
        if (suc == nil)
            return {K{}, nullptr};
        return {suc->key, &suc->data};
    }

    // В противном случае следующим по величине элементом является тот
    //  предок, для которого ветвь исходного элемента является левой
    while (node->parent != nil) {
        if (node->parent->left == node)
            return {node->parent->key, &node->parent->data};
        node = node->parent;
    }

    return {K{}, nullptr};
}

// Предыдущий по величине ключа элемент
template <typename K, typename T>
pair<K, T*> BinTree<K, T>::predecessor(K key) const {
    _Node<K, T>* node = search_node(key);
    if (node == nil)
        return {K{}, nullptr};

    // Если у узла есть левое поддерево, то предыдущим элементом
    // будет максимальный элемент левого поддерева
    if (node->left != nil) {
        auto pred = maximum_node(node->left);
        if (pred == nil)
            return {K{}, nullptr};
        return {pred->key, &pred->data};
    }

        // В противном случае предыдущим по величине элементом является тот
        //  предок, для которого ветвь исходного элемента является правой
        while (node->parent != nil) {
            if (node->parent->right == node)
                return {node->parent->key, &node->parent->data};
            node = node->parent;
        }

        return {K{}, nullptr};
}




template <typename K, typename T>
void BinTree<K, T>::recursive_delete(_Node<K, T>* node) {
    if (node == nil)
        return;

    recursive_delete(node->left);
    recursive_delete(node->right);
    delete node;
}


// Ищет узел, необходима для других функций
template <typename K, typename T>
_Node<K, T>* BinTree<K, T>::search_node(K key) const {
    // Поиск по двоичному дереву
    auto node = root;
    while (node != nil && node->key != key) {
        if (key < node->key)
            node = node->left;
        else
            node = node->right;
    }

    return node;
}


// Поместить поддерево с корнем элемент вместо элемента position
// Функция НЕ занимается удалением поддерева position.
template <typename K, typename T>
void BinTree<K, T>::transplant(_Node<K, T>* position, _Node<K, T>* element) {
    if (position->parent == nil)
        root = element;
    else if (position->parent->left == position)
        position->parent->left = element;
    else
        position->parent->right = element;

    if (element != nil)
        element->parent = position->parent;
}

template <typename K, typename T>
BinTree<K, T>::~BinTree() {
    recursive_delete(root);
    delete nil;
}


/*template <typename K, typename T>
std::ostream& operator<<(std::ostream &os, const RBTree<K, T> &tree) {
    tree.print_from_node(os, tree.root);
    return os;
    }*/

// Симметричный обход
template <typename K, typename T>
void BinTree<K, T>::print() const {
    return print_from_node(std::cout, root);
}


template <typename K, typename T>
void BinTree<K, T>::print_from_node(std::ostream &os, _Node<K, T>* node) const {
    // Симметричный обход массива
    if (node == nil)
        return;

    print_from_node(os, node->left);
    os << " (" << node->key << ", " << node->data << ") ";
    print_from_node(os, node->right);
}


template <typename K, typename T>
std::pair<K, T*> BinTree<K, T>::operator[](K key) {
    return search(key);
}

template <typename K, typename T>
std::vector<Node<K, T>> BinTree<K, T>::get_static() const {
    std::vector<Node<K, T>> result;
    get_static(result, root, 1, 0);
    return result;
}

template <typename K, typename T>
void BinTree<K,T>::get_static(std::vector<Node<K, T>> &arr, _Node<K, T>* node, int deep, int id) const {
    if (node != nil) {
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

template <typename K, typename T>
int BinTree<K, T>::height() const {
   return height(root);
}

template <typename K, typename T>
int BinTree<K, T>::height(_Node<K, T> *node) const {
    if (node == nil)
        return 0;
    return 1 + std::max(height(node->left), height(node->right));
}
