#pragma once

#include <array>
#include <cstdint>

namespace Moss {
class Cpu final
{
  public:
    Cpu() = default;

    [[nodiscard]] std::uint16_t Pc() const { return pc; }
    void Pc(std::uint16_t pc) { this->pc = pc; }

    [[nodiscard]] std::uint8_t A() const { return a; }
    void A(std::uint8_t a) { this->a = a; }

    [[nodiscard]] std::uint8_t X() const { return x; }
    void X(std::uint8_t x) { this->x = x; }

    [[nodiscard]] std::uint8_t Y() const { return y; }
    void Y(std::uint8_t y) { this->y = y; }

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

    std::uint8_t Read(std::uint16_t addr) const { return ram[addr]; }
    void Write(std::uint16_t addr, std::uint8_t data) { ram[addr] = data; }

    void Step();

  private:
    static constexpr std::size_t ADDR_SPACE_SIZE = 0x10000;

    struct Status
    {
        bool c{ false };
        bool z{ false };
        bool i{ true };
        bool d{ false };
        bool b{ true };
        bool u{ true };
        bool v{ false };
        bool n{ false };
    };

    std::uint16_t pc{ 0 };
    std::uint8_t a{ 0 };
    std::uint8_t x{ 0 };
    std::uint8_t y{ 0 };
    std::uint8_t s{ 0xFD };
    Status p{};

    std::array<std::uint8_t, ADDR_SPACE_SIZE> ram{};

    template<bool write>
    std::uint16_t Abx()
    {
        std::uint8_t low;
        auto overflow = __builtin_add_overflow(Read(pc++), x, &low);
        std::uint8_t high = Read(pc++);

        if constexpr (write) {
            Read(static_cast<uint16_t>(low | high << 8));
        } else if (overflow) {
            Read(static_cast<uint16_t>(low | high << 8));
        }

        return static_cast<uint16_t>(low | (high + overflow) << 8);
    }

    template<bool write>
    std::uint16_t Aby()
    {
        std::uint8_t low;
        auto overflow = __builtin_add_overflow(Read(pc++), x, &low);
        auto high = Read(pc++);

        if constexpr (write) {
            Read(static_cast<uint16_t>(low | high << 8));
        } else if (overflow) {
            Read(static_cast<uint16_t>(low | high << 8));
        }

        return static_cast<uint16_t>(low | (high + overflow) << 8);
    }

    std::uint16_t Imm() { return pc++; };

    std::uint16_t Ind()
    {
        auto ptr_low = Read(pc++);
        auto ptr_high = Read(pc++);
        auto low = Read(static_cast<uint16_t>(ptr_low | ptr_high << 8));
        auto high = Read(static_cast<uint16_t>((ptr_low + 1) | ptr_high << 8));
        return static_cast<uint16_t>(low | high << 8);
    }

    std::uint16_t Idx()
    {
        auto ptr = Read(pc++);
        Read(ptr);
        ptr += x;
        auto low = Read(ptr);
        auto high = Read(ptr + 1);
        return static_cast<uint16_t>(low | high << 8);
    }

    template<bool write>
    std::uint16_t Idy()
    {
        auto ptr = Read(pc++);
        std::uint8_t low;
        auto overflow = __builtin_add_overflow(Read(ptr), y, &low);
        auto high = Read(ptr + 1);

        if constexpr (write) {
            Read(static_cast<uint16_t>(low | high << 8));
        } else if (overflow) {
            Read(static_cast<uint16_t>(low | high << 8));
        }

        return static_cast<uint16_t>(low | (high + overflow) << 8);
    }

    std::uint16_t Zpg() { return Read(pc++); }

    std::uint16_t Zpx()
    {
        auto addr = Read(pc++);
        Read(addr);
        return addr + x;
    }

    std::uint16_t Zpy()
    {
        auto addr = Read(pc++);
        Read(addr);
        return addr + y;
    }

    void Lda(std::uint16_t addr)
    {
        a = Read(addr);
        p.z = a == 0;
        p.n = (a & 0x80) != 0;
    };
};
}
