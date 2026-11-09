#include "tree.h"
#include <iostream>
#include <queue>
#include <sstream>
#include <utility>

Tree Tree_create() { return Tree{}; }

static void destroy_node(TreeNode* node) {
    if (!node) return;
    destroy_node(node->left);
    destroy_node(node->right);
    delete node;
}
void Tree_destroy(Tree& tree) { destroy_node(tree.root); tree.root = nullptr; }

// BFS finds the first free child position, preserving completeness.
void Tree_insert(Tree& tree, int data) {
    if (!tree.root) { tree.root = new TreeNode{data, nullptr, nullptr}; return; }
    std::queue<TreeNode*> pending;
    pending.push(tree.root);
    while (!pending.empty()) {
        TreeNode* node = pending.front(); pending.pop();
        if (!node->left) { node->left = new TreeNode{data, nullptr, nullptr}; return; }
        if (!node->right) { node->right = new TreeNode{data, nullptr, nullptr}; return; }
        pending.push(node->left); pending.push(node->right);
    }
}
bool Tree_find(const Tree& tree, int data) {
    std::queue<const TreeNode*> pending;
    if (tree.root) pending.push(tree.root);
    while (!pending.empty()) {
        auto node = pending.front(); pending.pop();
        if (node->data == data) return true;
        if (node->left) pending.push(node->left);
        if (node->right) pending.push(node->right);
    }
    return false;
}
// Once a missing position is seen in BFS, every later position must be empty.
bool Tree_is_complete(const Tree& tree) {
    std::queue<const TreeNode*> pending;
    pending.push(tree.root);
    bool gap = false;
    while (!pending.empty()) {
        auto node = pending.front(); pending.pop();
        if (!node) { gap = true; continue; }
        if (gap) return false;
        pending.push(node->left); pending.push(node->right);
    }
    return true;
}
std::string Tree_read(const Tree& tree) {
    std::ostringstream out;
    std::queue<const TreeNode*> pending;
    if (tree.root) pending.push(tree.root);
    while (!pending.empty()) {
        auto node = pending.front(); pending.pop();
        out << " " << node->data;
        if (node->left) pending.push(node->left);
        if (node->right) pending.push(node->right);
    }
    return out.str();
}
void Tree_print(const Tree& tree) {
    if (!tree.root) { std::cout << "(empty)\n"; return; }
    std::queue<std::pair<const TreeNode*, std::string>> pending;
    pending.push({tree.root, "root"});
    while (!pending.empty()) {
        auto item = pending.front(); pending.pop();
        auto node = item.first;
        std::cout << item.second << " = " << node->data << "\n";
        if (node->left) pending.push({node->left, item.second + "->left"});
        if (node->right) pending.push({node->right, item.second + "->right"});
    }
}
