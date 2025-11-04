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

std::string generateImgName(const std::string& prefix="image", const std::string& extension="png") {
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;
    ss << std::put_time(std::localtime(&time_t), "%Y-%m-%d_%H-%M-%S");
    return prefix + "_" + ss.str();
}

int main(int argc, char** argv) {
    AVLTree<int, funcIt> tree(compareInt);
    // CLI::App app {BLUE "avl tre ><><><>" RESET};
    // argv = app.ensure_utf8(argv);
    // CLI11_PARSE(app, argc, argv);

    for (int wrd = 0; wrd < argc; ++wrd) {
        if (!strcmp(argv[wrd], "k")) {
            int key = std::stoi(argv[wrd+1]);
            tree.insert(key);
            wrd++;
            // fprintf(stdout, "%d\n", key);

        } else if (!strcmp(argv[wrd], "q")) {
            int lower_bound  = std::stoi(argv[wrd+1]);
            int upper_bound = std::stoi(argv[wrd+2]);
            int32_t range_count = tree.countRange(lower_bound, upper_bound);

            wrd += 2;
            fprintf(stdout, "%d ", range_count);
        }
    }
    fprintf(stdout, "\n");
    if (!strcmp(argv[1], "--dump")) tree.dumpTree(tree.getTopNode(), generateImgName());

}
