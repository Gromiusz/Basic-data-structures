#include "lib.hpp"

BinaryTree::BinaryTree(int _v) : value(_v) {
    left = nullptr;
    right = nullptr;
}

BinaryTree::~BinaryTree(){
    delete left;
    delete right;
}

bool BinaryTreeMethods::is_right_nullptr(BinaryTree * node){
    if(!node->right){
        return true;
    }
    return false;
}

bool BinaryTreeMethods::is_left_nullptr(BinaryTree * node){
    if(!node->left){
        return true;
    }
    return false;
}

int BinaryTreeMethods::get_value(BinaryTree * node){
    return node->value;
}

void BinaryTreeMethods::insert(BinaryTree * root, int _v)
{
    if(root->value <= _v){
        if(root->right == nullptr){
            root->right = new BinaryTree(_v);
        }
        else{
            BinaryTreeMethods::insert(root->right, _v);
        }
    }
    if(root->value > _v){
        if(root->left == nullptr){
            root->left = new BinaryTree(_v);
        }
        else{
            BinaryTreeMethods::insert(root->left, _v);
        }
    }
}

BinaryTree* BinaryTreeMethods::search(BinaryTree * root, int _v)
{
    if(_v == root->value) {
        return root;
    }
    else if(_v > root->value){
        if(root->right){
            return BinaryTreeMethods::search(root->right, _v);
        }
        else {
            return nullptr;
        }
    }
    else {
        if(root->left){
            return BinaryTreeMethods::search(root->left, _v);
        }
        else {
            return nullptr;
        }
    }
}

int BinaryTreeMethods::delete_value(BinaryTree * root, int _v)
{
    BinaryTree * node_to_delete = BinaryTreeMethods::search(root, _v);
    if(!node_to_delete) {
        std::cerr << "That value doesn't exist in this tree" << std::endl;
        return -1;
    }
    else if(!node_to_delete->left && !node_to_delete->right) {
        if(BinaryTreeMethods::clear_pointer_to_node(root, _v) == -1){
            std::cerr << "Error while clearing pointer" << std::endl;
            return -1;
        }
        delete node_to_delete;
        return 0;

    }
    else if(node_to_delete->left && !node_to_delete->right) {
        if(BinaryTreeMethods::replace_pointer_to_node(root, node_to_delete->left, _v) == -1){
            std::cerr << "Error while switching to left node pointer" << std::endl;
            return -1;
        }
        delete node_to_delete;
        return 0;
    }
    else if(!node_to_delete->left && node_to_delete->right) {
        if(BinaryTreeMethods::replace_pointer_to_node(root, node_to_delete->right, _v) == -1){
            std::cerr << "Error while switching to right node pointer" << std::endl;
            return -1;
        }
        delete node_to_delete;
        return 0;
    }
    else if(node_to_delete->left && node_to_delete->right) {
        if(!node_to_delete->left->right){
            if(BinaryTreeMethods::replace_pointer_to_node(root, node_to_delete->left, _v) == -1){
                std::cerr << "Error while switching to right node pointer" << std::endl;
                return -1;
            }
            node_to_delete->left->right = node_to_delete->right;
            delete node_to_delete;
            return 0;
        }
        else{
            node_to_delete->left->right->left = node_to_delete->left;
            node_to_delete->left->right->right = node_to_delete->left;
        }
    }
}

int BinaryTreeMethods::clear_pointer_to_node(BinaryTree * root, int _v)
{
    if(_v > root->value){
        if(root->right){
            if(root->right->value == _v) {
                root->right = nullptr;
                return 0;
            }
            return BinaryTreeMethods::clear_pointer_to_node(root->right, _v);
        }
        else {
            return -1;
        }
    }
    else {
        if(root->left){
            if(root->left->value == _v) {
                root->left = nullptr;
                return 0;
            }
            return BinaryTreeMethods::clear_pointer_to_node(root->left, _v);
        }
        else {
            return -1;
        }
    }
}

int BinaryTreeMethods::replace_pointer_to_node(BinaryTree * root, BinaryTree * new_pointer, int _v)
{
    if(_v > root->value){
        if(root->right){
            if(root->right->value == _v) {
                root->right = new_pointer;
                return 0;
            }
            return BinaryTreeMethods::replace_pointer_to_node(root->right, new_pointer, _v);
        }
        else {
            return -1;
        }
    }
    else {
        if(root->left){
            if(root->left->value == _v) {
                root->left = new_pointer;
                return 0;
            }
            return BinaryTreeMethods::replace_pointer_to_node(root->left, new_pointer, _v);
        }
        else {
            return -1;
        }
    }
}

// int BinaryTreeMethods::replace_pointers(){

// }