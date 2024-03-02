#include "cpu.h"

#include <algorithm>
#include <fstream>
#include <memory>

#include <gtest/gtest.h>

namespace {

TEST(Klaus, Functional) {
    auto cpu = std::make_unique<moss::Cpu>();
    std::ifstream rom{"../../6502_functional_test.bin", std::ios::binary};
    std::uint16_t addr = 0x000A;
    std::for_each(std::istreambuf_iterator<char>{rom},
                  std::istreambuf_iterator<char>{},
                  [&](std::uint8_t data) { cpu->Write(addr++, data); });

    cpu->Reset();
    cpu->Pc(0x0400);
    auto prev_pc = cpu->Pc();
    while (true) {
        cpu->Step();

        if (prev_pc == cpu->Pc()) {
            ASSERT_EQ(cpu->Pc(), 0x336D);
            break;
        }

        prev_pc = cpu->Pc();
    }
}

}
