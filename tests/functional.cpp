#include <fstream>
#include <iostream>
#include <iterator>
#include <memory>
#include <vector>

#include <doctest/doctest.h>

#include "moss/cpu.h"
#include "moss/types.h"

namespace moss {
namespace {

TEST_CASE("functional") {
    constexpr u16 ZeroPageAddr = 0x000A;
    constexpr u16 CodeAddr = 0x0400;
    constexpr u16 SuccessAddr = 0x336D;

    std::ifstream ifs("../../roms/6502_functional_test.bin",
                      std::ios::in | std::ios::binary);
    std::vector<u8> rom(std::istreambuf_iterator<char>(ifs), {});

    auto cpu = std::make_unique<Cpu>();
    REQUIRE(cpu->Load(rom, ZeroPageAddr));
    cpu->Pc(CodeAddr);

    u16 prev_pc;
    do {
        prev_pc = cpu->Pc();
        cpu->Step();
    } while (prev_pc != cpu->Pc());

    REQUIRE(cpu->Pc() == SuccessAddr);
}

}
}