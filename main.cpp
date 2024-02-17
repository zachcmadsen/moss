#include <cassert>
#include <memory>

#include "cpu.h"

int
main()
{
    auto cpu = std::make_unique<Moss::Cpu>();

    cpu->Pc(0x0200);

    cpu->Write(0x0200, 0xA9);
    cpu->Write(0x0201, 0xEF);

    cpu->Step();

    assert(cpu->A() == 0xEF);

    return 0;
}
