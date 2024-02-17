#include "cpu.h"

#include <cstdint>

namespace Moss {
void
Cpu::step()
{
    const std::uint8_t opc = ram.at(pc++);
    switch (opc) {
        case 0xA9:
            const std::uint16_t addr = imm();
            lda(addr);
            break;
    }
}
}
