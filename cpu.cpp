#include "cpu.h"

#include <cstdint>

namespace Moss {
void
Cpu::Step()
{
    const std::uint8_t opc = ram.at(pc++);
    switch (opc) {
        case 0xA9:
            Lda(Imm());
            break;
    }
}
}
