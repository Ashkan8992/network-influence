#include <iostream>

#include "influence/version.hpp"

int main () {
    std::cout << "Network Influence Simulator v"
        << influence::version() << '\n';
    return 0;
}
