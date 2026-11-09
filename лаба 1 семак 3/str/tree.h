#ifndef TREE_H
#define TREE_H
#include <string>

struct TreeNode {
    int data;
    TreeNode* left;
    TreeNode* right;
};
struct Tree { TreeNode* root = nullptr; };
Tree Tree_create();
void Tree_destroy(Tree& tree);
void Tree_insert(Tree& tree, int data);
bool Tree_find(const Tree& tree, int data);
bool Tree_is_complete(const Tree& tree);
std::string Tree_read(const Tree& tree);
void Tree_print(const Tree& tree);
#endif
