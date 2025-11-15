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
            test_set.insert(key);
        } else if (command == "q") {
            int lower_bound = 0;
            int upper_bound = 0;

            if (std::cin >> lower_bound >> upper_bound) {
                auto lowIt = test_set.lower_bound(lower_bound);
                auto upIt  = test_set.upper_bound(upper_bound);
                if (lowIt != test_set.end() && upIt != test_set.end())
                {
                    if (lower_bound <= upper_bound) {
                        auto distance = std::distance(lowIt, upIt);
                    } else {
                        auto distance = std::distance(upIt, lowIt);
                    }
                    // std::cout << distance << "\n";
                }
            }
        }
    }

    auto end = Clock::now();

    std::cout << end - start << std::endl;
    // std::cout << std::endl;
}
