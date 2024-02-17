#include <cassert>
#include <memory>

#include "cpu.h"

int
main()
{
    auto cpu = std::make_unique<Moss::Cpu>();

    cpu->pc = 0x0200;

    cpu->ram[0x0200] = 0xA9;
    cpu->ram[0x0201] = 0xEF;

    cpu->step();

    assert(cpu->a == 0xEF);

    return 0;
}
