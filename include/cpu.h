#pragma once

#include <array>
#include <cstdint>

#include "integer.h"

namespace moss {

class Cpu final {
  public:
    Cpu() = default;

    /// TODO
    [[nodiscard]] u8 A() const;

    /// TODO
    void A(u8 a);

    /// TODO
    [[nodiscard]] u8 X() const;

    /// TODO
    void X(u8 x);

    /// TODO
    [[nodiscard]] u8 Y() const;

    /// TODO
    void Y(u8 y);

    /// Returns the contents of the PC register.
    [[nodiscard]] u16 Pc() const;

    /// Sets the PC register to `pc`.
    void Pc(u16 pc);

    /// TODO
    [[nodiscard]] u8 S() const;

    /// TODO
    void S(u8 s);

    /// TODO
    [[nodiscard]] u8 P() const;

    /// TODO
    void P(u8 p);

    /// TODO
    u8 Read(u16 addr) const;

    /// TODO
    void Write(u16 addr, u8 data);

    // TODO: Add a loam rom function?

    /// Runs the reset sequence. Note, the CPU doesn't execute the sequence the
    /// first time `Step` is called. You have to do it manually by calling
    /// this function.
    void Reset();

    /// Steps the CPU by one instruction.
    void Step();

  private:
    struct Status {
        Status() = default;
        explicit Status(std::uint8_t p)
            : c{(p & 0x01) != 0}, z{(p & 0x02) != 0}, i{(p & 0x04) != 0},
              d{(p & 0x08) != 0}, b{(p & 0x10) != 0}, u{(p & 0x20) != 0},
              v{(p & 0x40) != 0}, n{(p & 0x80) != 0} {
        }

        explicit operator std::uint8_t() const {
            return static_cast<std::uint8_t>(c | (z << 1) | (i << 2) |
                                             (d << 3) | (b << 4) | (u << 5) |
                                             (v << 6) | (n << 7));
        }

        bool c{false};
        bool z{false};
        bool i{true};
        bool d{false};
        bool b{true};
        bool u{true};
        bool v{false};
        bool n{false};
    };

    static constexpr std::size_t AddrSpaceSize = 0x10000;
    static constexpr std::size_t StackAddr = 0x0100;

    std::uint16_t pc{0};
    std::uint8_t a{0};
    std::uint8_t x{0};
    std::uint8_t y{0};
    std::uint8_t s{0xFD};
    Status p{};

    // SHA/SHX/SHY instructions need to know if there was a page cross while
    // getting the effective address.
    bool page_cross;

    std::array<std::uint8_t, AddrSpaceSize> ram{};

    void Add(std::uint8_t data) {
        auto prev_a = a;
        auto sum = a + data + p.c;
        a = static_cast<std::uint8_t>(sum);
        p.c = sum > 0xFF;
        p.v = (prev_a ^ a) & (data ^ a) & 0x80;
        p.z = !a;
        p.n = a & 0x80;
    }

    void Branch(bool cond) {
        auto offset = Read(pc++);
        if (cond) {
            Read(pc);

            auto prev_pc = pc;
            pc += static_cast<std::int8_t>(offset);

            if ((prev_pc & 0xFF00) != (pc & 0xFF00)) {
                Read(static_cast<std::uint8_t>(prev_pc + offset) |
                     (prev_pc & 0xFF00));
            }
        }
    }

    void Compare(uint8_t left, uint8_t right) {
        uint8_t diff;
        bool overflow = __builtin_sub_overflow(left, right, &diff);
        p.c = !overflow;
        p.z = !diff;
        p.n = diff & 0x80;
    }

    std::uint8_t Peek() const {
        return Read(StackAddr + s);
    }

    std::uint8_t Pop() {
        ++s;
        return Read(StackAddr + s);
    }

    void Push(std::uint8_t data) {
        Write(StackAddr + s, data);
        --s;
    }

    void ShInner(std::uint16_t addr, std::uint8_t reg) {
        std::uint8_t high = addr >> 8;
        std::uint8_t thing = reg & (high + !page_cross);
        high = page_cross ? thing : high;
        Write(static_cast<std::uint16_t>((addr & 0x00FF) | high << 8), thing);
    }

    // Addressing modes
    u16 Abs();
    template <bool write> u16 Abx();
    template <bool write> u16 Aby();
    u16 Idx();
    template <bool write> u16 Idy();
    u16 Imm();
    u16 Imp();
    u16 Ind();
    u16 Zpg();
    u16 Zpx();
    u16 Zpy();

    // Instructions
    void Adc(u16 addr);
    void Alr(u16 addr);
    void Anc(u16 addr);
    void And(u16 addr);
    void Arr(u16 addr);
    void Asl(u16 addr);
    void AslA(u16 addr);
    void Bcc(u16 addr);
    void Bcs(u16 addr);
    void Beq(u16 addr);
    void Bit(u16 addr);
    void Bmi(u16 addr);
    void Bne(u16 addr);
    void Bpl(u16 addr);
    void Brk(u16 addr);
    void Bvc(u16 addr);
    void Bvs(u16 addr);
    void Clc(u16 addr);
    void Cld(u16 addr);
    void Cli(u16 addr);
    void Clv(u16 addr);
    void Cmp(u16 addr);
    void Cpx(u16 addr);
    void Cpy(u16 addr);
    void Dcp(u16 addr);
    void Dec(u16 addr);
    void Dex(u16 addr);
    void Dey(u16 addr);
    void Eor(u16 addr);
    void Inc(u16 addr);
    void Inx(u16 addr);
    void Iny(u16 addr);
    void Isc(u16 addr);
    void Jmp(u16 addr);
    void Jsr(u16 addr);
    void Las(u16 addr);
    void Lax(u16 addr);
    void Lda(u16 addr);
    void Ldx(u16 addr);
    void Ldy(u16 addr);
    void Lsr(u16 addr);
    void LsrA(u16 addr);
    void Nop(u16 addr);
    void Ora(u16 addr);
    void Pha(u16 addr);
    void Php(u16 addr);
    void Pla(u16 addr);
    void Plp(u16 addr);
    void Rla(u16 addr);
    void Rol(u16 addr);
    void RolA(u16 addr);
    void Ror(u16 addr);
    void RorA(u16 addr);
    void Rra(u16 addr);
    void Rti(u16 addr);
    void Rts(u16 addr);
    void Sax(u16 addr);
    void Sbc(u16 addr);
    void Sbx(u16 addr);
    void Sec(u16 addr);
    void Sed(u16 addr);
    void Sei(u16 addr);
    void Sha(u16 addr);
    void Shx(u16 addr);
    void Shy(u16 addr);
    void Slo(u16 addr);
    void Sre(u16 addr);
    void Sta(u16 addr);
    void Stx(u16 addr);
    void Sty(u16 addr);
    void Tas(u16 addr);
    void Tax(u16 addr);
    void Tay(u16 addr);
    void Tsx(u16 addr);
    void Txa(u16 addr);
    void Txs(u16 addr);
    void Tya(u16 addr);
};

inline u8 Cpu::A() const {
    return a;
}

inline void Cpu::A(u8 a) {
    this->a = a;
}

inline u8 Cpu::X() const {
    return x;
}

inline void Cpu::X(u8 x) {
    this->x = x;
}

inline u8 Cpu::Y() const {
    return y;
}

inline void Cpu::Y(u8 y) {
    this->y = y;
}

inline u16 Cpu::Pc() const {
    return pc;
}

inline void Cpu::Pc(u16 pc) {
    this->pc = pc;
}

inline u8 Cpu::S() const {
    return s;
}

inline void Cpu::S(u8 s) {
    this->s = s;
}

inline u8 Cpu::P() const {
    return u8(p);
}

inline void Cpu::P(u8 p) {
    this->p = Status(p);
}

inline u8 Cpu::Read(u16 addr) const {
    return ram[addr];
}

inline void Cpu::Write(u16 addr, u8 data) {
    ram[addr] = data;
};

}
