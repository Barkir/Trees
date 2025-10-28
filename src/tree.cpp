#include <iostream>
#include <functional>

#include "tree.hpp"
#include "CLI11.hpp"

int compareInt(int a, int b) {
    return a > b;
}

using funcIt = std::function<int(int, int)>;

int main() {
    AVLTree<int, funcIt> tree(compareInt);
    tree.insert(100);
    tree.insert(110);
    tree.insert(15);
    tree.insert(30);
    tree.insert(11);
    // tree.insert(10);
    // tree.insert(1);
    // tree.insert(2);
    // tree.insert(3);
    // tree.insert(9);
    // tree.insert(11);
    // tree.insert(25);
    // tree.insert(24);
    // tree.insert(12);
    // tree.insert(15);
    tree.dumpTree(tree.getTopNode(), "image");

}
