#include "tree.h"
#include <cassert>
#include <iostream>
int main() {
    Tree t=Tree_create();
    assert(Tree_is_complete(t));
    for(int i=0;i<100;++i) {
        Tree_insert(t,i);
        assert(Tree_is_complete(t));
        assert(Tree_find(t,i));
    }
    assert(!Tree_find(t,-1));
    Tree_destroy(t);
    // Right child without a left child.
    t.root=new TreeNode{1,nullptr,new TreeNode{2,nullptr,nullptr}};
    assert(!Tree_is_complete(t)); Tree_destroy(t);
    // A gap earlier on the last level, then another node to its right.
    t.root=new TreeNode{1,new TreeNode{2,nullptr,nullptr},new TreeNode{3,new TreeNode{4,nullptr,nullptr},nullptr}};
    assert(!Tree_is_complete(t)); Tree_destroy(t);
    // Complete does not require every internal node to have two children.
    t.root=new TreeNode{1,new TreeNode{2,nullptr,nullptr},nullptr};
    assert(Tree_is_complete(t)); Tree_destroy(t);
    std::cout<<"PASS: tree insertion, search and complete/incomplete shapes\n";
}
