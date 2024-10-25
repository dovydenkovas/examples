#pragma once
#include <utility>
#include <cmath>


#include "bintree.hpp"

using std::pair;


template <typename K, typename T>
class RBTree: public BinTree<K, T> {
public:
    RBTree();
    void insert(K key, T data);
    void remove(K key);

private:
    void left_rotate(_Node<K, T> *node);
    void right_rotate(_Node<K, T> *node);
    void insert_fixup(_Node<K, T> *node);
};


template <typename K, typename T>
RBTree<K, T>::RBTree() {
    this->nil = new _Node<K, T>{{}, {}};
    this->nil->color = 'b';
    this->root = this->nil;
}

template <typename K, typename T>
void RBTree<K, T>::insert(K key, T data) {
    // Создаем новый узел
    _Node<K, T> *new_node = new _Node{key, data, this->nil, this->nil};
    new_node->color = 'r';

    // Ищем позицию для вставки
    _Node<K, T> *node = this->root;
    _Node<K, T> *parent = this->nil;
    while (node != this->nil) {
        parent = node;
        if (key < node->key)
            node = node->left;
        else
            node = node->right;
    }

    // Вставляем новый узел
    new_node->parent = parent;
    if (parent == this->nil)
        this->root = new_node;
    else if (key < parent->key)
        parent->left = new_node;
    else
        parent->right = new_node;

    // Восстанавливаем красно-черные свойства
    insert_fixup(new_node);
}


template <typename K, typename T>
void RBTree<K, T>::left_rotate(_Node<K, T> *node) {
    // Новый корень
    _Node<K, T> *new_root = node->right;

    // Связь с родителем поддерева
    new_root->parent = node->parent;
    if (new_root->parent != this->nil) {
        if (node->parent->left == node)
            new_root->parent->left = new_root;
        else
            new_root->parent->right = new_root;
    } else
        this->root = new_root;

    // Превращение левого поддерева нового корня
    // в правое поддерево старого.
    node->right = new_root->left;
    if (node->right != this->nil)
        node->right->parent = node;

    // Смена взаимоотношений старого и нового корней.
    new_root->left = node;
    node->parent = new_root;
}


template <typename K, typename T>
void RBTree<K, T>::right_rotate(_Node<K, T> *node) {
    // Новый корень
    _Node<K, T> *new_root = node->left;

    // Связь с родителем поддерева
    new_root->parent = node->parent;
    if (new_root->parent != this->nil) {
        if (node->parent->left == node)
            new_root->parent->left = new_root;
        else
            new_root->parent->right = new_root;
    } else
        this->root = new_root;

    // Превращение правого поддерева нового корня
    // в левое поддерево старого корня.
    node->left = new_root->right;
    if (node->left != this->nil)
        node->left->parent = node;

    // Смена взаимоотношений старого и нового корней.
    new_root->right = node;
    node->parent = new_root;
}


template <typename K, typename T>
void RBTree<K, T>::insert_fixup(_Node<K, T> *node) {
    while (node->parent->color == 'r') {
        // Узел в левом поддереве
        if (node->parent->parent->left == node->parent) {
            _Node<K, T> *uncle = node->parent->parent->right;
            if (uncle->color == 'r') {
                uncle->color = 'b';
                node->parent->color = 'b';
                node->parent->parent->color = 'r';
                node = node->parent->parent;
            } else {
                if (node == node->parent->right) {
                    node = node->parent;
                    left_rotate(node);
                }
                node->parent->color = 'b';
                node->parent->parent->color = 'r';
                right_rotate(node->parent->parent);
            }
        }
        // Узел в правом поддерерве
        else {
            _Node<K, T> *uncle = node->parent->parent->left;
            if (uncle->color == 'r') {
                node->parent->color = 'b';
                uncle->color = 'b';
                node->parent->parent->color = 'r';
                node = node->parent->parent;
            } else {
                if (node == node->parent->left) {
                    node = node->parent;
                    right_rotate(node);
                }
                node->parent->color = 'b';
                node->parent->parent->color = 'r';
                left_rotate(node->parent->parent);
            }
        }
    }

    this->root->color = 'b';
}

template <typename K, typename T>
void RBTree<K, T>::remove(K key) {
   // TODO
    auto node = this->search_node(key);
    if (node == nullptr)
        return;

    if (node->left == nullptr)
        this->transplant(node, node->right);
    else if (node->right == nullptr)
        this->transplant(node, node->left);

    else {
        auto min = this->minimum_node(node->right);
        if (min->parent != node) {
            this->transplant(min, min->right);
            min->right = node->right;
            min->right->parent = min;
        }

        this->transplant(node, min);
        min->left = node->left;
        min->left->parent = min;
    }

    delete  node;
}
