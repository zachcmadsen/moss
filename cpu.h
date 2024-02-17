#pragma once

#include <array>
#include <cstdint>

namespace Moss {
class Cpu
{
  public:
    [[nodiscard]] std::uint8_t A() const { return a; }
    void A(std::uint8_t a) { this->a = a; }

    [[nodiscard]] std::uint8_t X() const { return x; }
    void X(std::uint8_t x) { this->x = x; }

    [[nodiscard]] std::uint8_t Y() const { return y; }
    void Y(std::uint8_t y) { this->y = y; }

    [[nodiscard]] std::uint16_t Pc() const { return pc; }
    void Pc(std::uint16_t pc) { this->pc = pc; }

    [[nodiscard]] std::uint8_t S() const { return s; }
    void S(std::uint8_t s) { this->s = s; }

    [[nodiscard]] std::uint8_t P() const
    {
        return static_cast<std::uint8_t>(
          static_cast<int>(p.c) | (static_cast<int>(p.z) << 1) |
          (static_cast<int>(p.i) << 2) | (static_cast<int>(p.d) << 3) |
          (static_cast<int>(p.b) << 4) | (static_cast<int>(p.u) << 5) |
          (static_cast<int>(p.v) << 6) | (static_cast<int>(p.n) << 7));
    }
    void P(std::uint8_t p)
    {
        this->p.c = (p & 0x01) > 0;
        this->p.z = (p & 0x02) > 0;
        this->p.i = (p & 0x04) > 0;
        this->p.d = (p & 0x08) > 0;
        this->p.b = (p & 0x10) > 0;
        this->p.u = (p & 0x20) > 0;
        this->p.v = (p & 0x40) > 0;
        this->p.n = (p & 0x80) > 0;
    }

    [[nodiscard]] std::uint8_t Read(std::uint16_t addr) const
    {
        return ram.at(addr);
    }
    void Write(std::uint16_t addr, std::uint8_t data) { ram.at(addr) = data; }

    void Step();

  private:
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

    std::array<std::uint8_t, 0x10000> ram{};

    std::uint16_t Imm() { return pc++; };

    void Lda(std::uint16_t addr)
    {
        a = ram.at(addr);
        p.z = a == 0;
        p.n = (a & 0x80) != 0;
    };
};
}
