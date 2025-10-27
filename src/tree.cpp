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
    tree.insert(10);
    tree.insert(1);
    tree.insert(2);
    tree.insert(3);
    tree.insert(25);

    tree.dumpTree(tree.getTopNode(), "image");

}
