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
    tree.insert(50);
    tree.insert(40);
    tree.insert(30);
    tree.insert(60);
    tree.insert(70);
    tree.insert(35);
    tree.insert(37);
    tree.insert(65);
    tree.insert(63);
    tree.insert(10);
    tree.insert(20);
    tree.insert(25);
    tree.insert(55);
    tree.insert(57);
    tree.insert(75);
    tree.insert(80);
    tree.insert(77);
    tree.insert(5);
    tree.insert(15);
    tree.insert(85);

    tree.dumpTree(tree.getTopNode(), "image");

}
