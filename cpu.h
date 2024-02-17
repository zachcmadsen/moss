#pragma once

#include <array>
#include <cstdint>

namespace Moss {
class Cpu
{
  public:
    struct Status
    {
        bool c{};
        bool z{};
        bool i{};
        bool d{};
        bool b{};
        bool u{};
        bool v{};
        bool n{};
    };

    std::uint8_t a{};
    std::uint8_t x{};
    std::uint8_t y{};
    std::uint16_t pc{};
    std::uint8_t s{};
    Status p{};

    std::array<uint8_t, 0x10000> ram{};

    void step();

  private:
    uint16_t imm() { return pc++; };

    void lda(std::uint16_t addr)
    {
        a = ram.at(addr);
        p.z = a == 0;
        p.n = (a & 0x80) != 0;
    };
};
}
