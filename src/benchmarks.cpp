#include <iostream>
#include <functional>
#include <string>
#include <chrono>

#include "tree.hpp"
#include "CLI11.hpp"

int compareInt(int a, int b) {
    return a > b;
}

using funcIt = std::function<int(int, int)>;
using Clock = std::chrono::high_resolution_clock;

std::string generateImgName(const std::string& prefix="image", const std::string& extension="png") {
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;
    ss << std::put_time(std::localtime(&time_t), "%Y-%m-%d_%H-%M-%S");
    return prefix + "_" + ss.str();
}

int main(int argc, char** argv) {
    AVLTree<int, funcIt> tree(compareInt);
    bool dump_flag = false;

    for (int i = 1; i < argc; ++i) {
        if (!strcmp(argv[i], "--dump")) {
            dump_flag = true;
        }
    }

    std::string command;

    int count = 0;
    auto start = Clock::now();
    while (std::cin >> command && count < 10000) {
        // std::cout << count << std::endl;
        if (command == "k") {
            int key = 0;
            std::cin >> key;
            tree.insert(key);
        } else if (command == "q") {
            int lower_bound, upper_bound;
            std::cin >> lower_bound >> upper_bound;
            // std::cout << range_count << " ";
            if (lower_bound <= upper_bound) {
                auto distance = tree.countRange(lower_bound, upper_bound);
                // std::cout << distance << " ";
            } else {
                auto distance = tree.countRange(upper_bound, lower_bound);
                // std::cout << distance << " ";
            }
        }
        ++count;
    }

    auto end = Clock::now();

    std::cout << end - start << std::endl;
    // std::cout << std::endl;

    if (dump_flag) {
        tree.dumpTree(tree.getTopNode(), generateImgName());
    }
}
