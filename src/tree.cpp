#include <iostream>
#include <functional>

#include "tree.hpp"
#include "CLI11.hpp"

int compareInt(int a, int b) {
    return a > b;
}

using funcIt = std::function<int(int, int)>;

int main() {
    AVLTree<int, funcIt> tree;
    tree.insert(1);

}
