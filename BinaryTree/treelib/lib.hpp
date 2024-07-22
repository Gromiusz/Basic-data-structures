#pragma once
#include <iostream>

class BinaryTree{
    BinaryTree * left;
    BinaryTree * right;
    int value;

public:
    BinaryTree(int _v);
    ~BinaryTree();
    // void insert(BinaryTree * root, int _v);
    // BinaryTree search(BinaryTree * root, int _v);
    // int delete_value(BinaryTree * root, int _v);

    // bool is_right_nullptr();
    // bool is_left_nullptr();
    // int get_value();
    friend class BinaryTreeMethods;
};

class BinaryTreeMethods{
    BinaryTreeMethods() = delete;
    ~BinaryTreeMethods() = delete;
    static int clear_pointer_to_node(BinaryTree * root, int _v);
public:
    static void insert(BinaryTree * root, int _v);
    static BinaryTree* search(BinaryTree * root, int _v);
    static int delete_value(BinaryTree * root, int _v);

    static bool is_right_nullptr(BinaryTree * node);
    static bool is_left_nullptr(BinaryTree * node);
    static int get_value(BinaryTree * node);
    static int replace_pointer_to_node(BinaryTree * root, BinaryTree * new_pointer, int _v);
};

using BTM = BinaryTreeMethods;