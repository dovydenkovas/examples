#pragma once
#include <utility>
#include <cmath>
#include "bintree.hpp"
#include "node.hpp"

using std::pair;


template <typename K, typename T>
class RBTree: public BinTree<K, T> {
public:
    void insert(K key, T data);
    void remove(K key);

    using BinTree<K, T>::BinTree;

private:
    void left_rotate(_Node<K, T> *node);
    void right_rotate(_Node<K, T> *node);
    void insert_fixup(_Node<K, T> *node);
    void remove_fixup(_Node<K, T> *node);
};


template <typename K, typename T>
void RBTree<K, T>::insert(K key, T data) {
    _Node<K, T> *node = this->search_node(key);

    if (!node->is_nil()) {
        node->data = data;
        return;
    }

    node->some();
    node->color = RED;
    node->key = key;
    node->data = data;
    if (node->left == nullptr)
        node->left = new _Node<K, T>;
    if (node->right == nullptr)
        node->right = new _Node<K, T>;
    node->left->parent = node;
    node->right->parent = node;

    // Восстанавливаем красно-черные свойства
    insert_fixup(node);
}


template <typename K, typename T>
void RBTree<K, T>::left_rotate(_Node<K, T> *node) {
    // Новый корень
    _Node<K, T> *new_root = node->right;

    // Превращение левого поддерева нового корня
    // в правое поддерево старого.
    node->right = new_root->left;
    node->right->parent = node;


    // Связь с родителем поддерева
    new_root->parent = node->parent;
    if (new_root->parent->is_nil())
        this->root = new_root;
    else if (new_root->parent->left == node)
        new_root->parent->left = new_root;
    else
        new_root->parent->right = new_root;

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
    if (!new_root->parent->is_nil()) {
        if (node->parent->left == node)
            new_root->parent->left = new_root;
        else
            new_root->parent->right = new_root;
    } else
        this->root = new_root;

    // Превращение правого поддерева нового корня
    // в левое поддерево старого корня.
    node->left = new_root->right;
    node->left->parent = node;

    // Смена взаимоотношений старого и нового корней.
    new_root->right = node;
    node->parent = new_root;
}


template <typename K, typename T>
void RBTree<K, T>::insert_fixup(_Node<K, T> *node) {
    while (node->parent->color == RED) {
        // Узел в левом поддереве
        if (node->parent->parent->left == node->parent) {

            _Node<K, T> *uncle = node->parent->parent->right;
            if (uncle->color == RED) {
                uncle->color = BLACK;
                node->parent->color = BLACK;
                node->parent->parent->color = RED;
                node = node->parent->parent;
            } else {
                if (node == node->parent->right) {
                    node = node->parent;
                    left_rotate(node);
                }
                node->parent->color = BLACK;
                node->parent->parent->color = RED;
                right_rotate(node->parent->parent);
            }
        }
        // Узел в правом поддерерве
        else {

            _Node<K, T> *uncle = node->parent->parent->left;
            if (uncle->color == RED) {
                node->parent->color = BLACK;
                uncle->color = BLACK;
                node->parent->parent->color = RED;
                node = node->parent->parent;
            } else {
                if (node == node->parent->left) {
                    node = node->parent;
                    right_rotate(node);
                }
                node->parent->color = BLACK;
                node->parent->parent->color = RED;
                left_rotate(node->parent->parent);
            }
        }
    }

    this->root->color = BLACK;
}

template <typename K, typename T>
void RBTree<K, T>::remove(K key) {
    _Node<K, T> *node = this->search_node(key);
    if (node->is_nil())
        return;

    Color original_color = node->color;
    _Node<K, T> *x;

    if (node->left->is_nil()) {
        delete node->left; // nil
        x = node->right;
        this->transplant(node, node->right);
    } else if (node->right->is_nil()) {
        delete node->right; // nil
        x = node->left;
        this->transplant(node, node->left);
    } else {
        auto min = this->minimum_node(node->right);
        delete min->left; // nil

        x = min->right;
        original_color = min->color;
        if (min->parent == node)
            x->parent = min;
        else {
            this->transplant(min, min->right);
            min->right = node->right;
            min->right->parent = min;
        }

        this->transplant(node, min);
        min->left = node->left;
        min->left->parent = min;
        min->color = node->color;
    }

    if (node != this->root)
        delete node;
    else
        node->nil();
    if (original_color == BLACK)
        remove_fixup(x);
}


template <typename K, typename T>
void RBTree<K, T>::remove_fixup(_Node<K, T> *node) {
    while (node->color == BLACK && node != this->root) {
        // Левый потомок
        if (node == node->parent->left) {
            _Node<K, T> *brother = node->parent->right;
            if (brother->color == RED) {
               brother->color = BLACK;
               node->parent->color = RED;
               left_rotate(node->parent);
               brother = node->parent->right;
            }

            if (brother->left->color == BLACK &&
               brother->right->color == BLACK) {
                   brother->color = RED;
                   node = node->parent;
               }
            else {
                if (brother->right->color == BLACK) {
                    brother->left->color = BLACK;
                    brother->color = RED;
                    right_rotate(brother);
                    brother = node->parent->right;
                }
                brother->color = node->parent->color;
                node->parent->color = BLACK;
                brother->right->color = BLACK;
                left_rotate(node->parent);
                node = this->root;
            }
        }
        // Правый потомок
        else {
            _Node<K, T> *brother = node->parent->left;
            if (brother->color == RED) {
                brother->color = BLACK;
                node->parent->color = RED;
                right_rotate(node->parent);
                brother = node->parent->left;
            }

            if (brother->right->color == BLACK &&
                brother->left->color == BLACK) {
                    brother->color = RED;
                    node = node->parent;
            } else {
                if (brother->left->color == BLACK) {
                    brother->right->color = BLACK;
                    brother->color = RED;
                    left_rotate(brother);
                    brother = node->parent->left;
                }
                    brother->color = node->parent->color;
                    node->parent->color = BLACK;
                    brother->left->color = BLACK;
                    right_rotate(node->parent);
                    node = this->root;
            }
        }
    }

    node->color = BLACK;
}
