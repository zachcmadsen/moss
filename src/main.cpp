#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>

#include "moss/moss.h"
#include "moss/types.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "moss: error: no input file\n";
        return 1;
    }

    auto filename = argv[1];
    std::ifstream ifs(filename, std::ios::in | std::ios::binary);
    if (!ifs.is_open()) {
        std::cerr << "moss: error: failed to open" << filename << '\n';
        return 1;
    }
    std::vector<moss::u8> rom(std::istreambuf_iterator<char>(ifs), {});

    auto result = moss::Run(rom);
    if (!result) {
        std::cerr << "moss: error: " << result.error() << '\n';
        return 1;
    }

    return 0;
}
