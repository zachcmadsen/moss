#include "cpu.h"

#include <fstream>
#include <memory>
#include <vector>

#include <gtest/gtest.h>

#include "integer.h"

namespace moss {

TEST(Klaus, Functional) {
    constexpr u16 ZeroPageAddr = 0x000A;
    constexpr u16 CodeAddr = 0x0400;
    constexpr u16 SuccessAddr = 0x336D;

    std::ifstream ifs{"../../6502_functional_test.bin",
                      std::ios::binary | std::ios::ate};
    auto file_size = ifs.tellg();
    std::vector<u8> rom(static_cast<std::size_t>(file_size));
    ifs.seekg(0, std::ios::beg);
    ifs.read(reinterpret_cast<char *>(rom.data()), file_size);

    auto cpu = std::make_unique<moss::Cpu>();
    cpu->Load(rom, ZeroPageAddr);
    cpu->Pc(CodeAddr);

    u16 prev_pc;
    do {
        prev_pc = cpu->Pc();
        cpu->Step();
    } while (prev_pc != cpu->Pc());

    ASSERT_EQ(cpu->Pc(), SuccessAddr);
}

}
