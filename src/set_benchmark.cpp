#include <iostream>
#include <functional>
#include <string>
#include <chrono>
#include <set>


using Clock = std::chrono::high_resolution_clock;

int main(int argc, char** argv) {

    std::set<int32_t> test_set;
    std::string command;

    auto start = Clock::now();
    while (std::cin >> command) {
        if (command == "k") {
            int key = 0;
            std::cin >> key;
            test_set.insert(key);
        } else if (command == "q") {
            int lower_bound, upper_bound;
            std::cin >> lower_bound >> upper_bound;

            auto lowIt = test_set.lower_bound(lower_bound);
            auto upIt  = test_set.upper_bound(upper_bound);
            int32_t distance = std::distance(lowIt, upIt);
            // std::cout << distance << " ";
        }
    }

    auto end = Clock::now();

    std::cout << end - start << std::endl;
    // std::cout << std::endl;
}
