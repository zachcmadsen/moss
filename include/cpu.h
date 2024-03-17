#pragma once

#include <array>
#include <span>

#include "integer.h"

namespace moss {

/// A 6502 emulator.
class Cpu final {
  public:
    /// Returns the accumulator.
    [[nodiscard]] u8 A() const;

    /// Sets the accumulator to `a`.
    void A(u8 a);

    /// Returns the X register.
    [[nodiscard]] u8 X() const;

    /// Sets the X register to `x`.
    void X(u8 x);

    /// Returns the Y register.
    [[nodiscard]] u8 Y() const;

    /// Sets the Y register to `y`.
    void Y(u8 y);

    /// Returns the program counter.
    [[nodiscard]] u16 Pc() const;

    /// Sets the program counter to `pc`.
    void Pc(u16 pc);

    /// Returns the stack pointer.
    [[nodiscard]] u8 S() const;

    /// Sets the stack pointer to `s`.
    void S(u8 s);

    /// Returns the status register.
    [[nodiscard]] u8 P() const;

    /// Sets the status register to `p`.
    void P(u8 p);

    /// Reads a byte from memory at `addr`.
    [[nodiscard]] u8 Read(u16 addr) const;

    /// Writes `data` to memory at `addr`.
    void Write(u16 addr, u8 data);

    /// Tries to load `rom` into memory at `offset`. Returns true if and only
    /// if `rom` was loaded.
    [[nodiscard]] bool Load(std::span<u8 const> rom, u16 offset);

    /// Steps execution by one instruction.
    void Step();

  private:
    struct Status {
        Status() = default;
        explicit Status(u8 p)
            : c{(p & 0x01) != 0}, z{(p & 0x02) != 0}, i{(p & 0x04) != 0},
              d{(p & 0x08) != 0}, b{(p & 0x10) != 0}, u{(p & 0x20) != 0},
              v{(p & 0x40) != 0}, n{(p & 0x80) != 0} {
        }

        explicit operator u8() const {
            return u8(c | (z << 1) | (i << 2) | (d << 3) | (b << 4) | (u << 5) |
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
    static constexpr u16 IrqVector = 0xFFFE;

    u16 pc{0};
    u8 a{0};
    u8 x{0};
    u8 y{0};
    u8 s{0xFD};
    Status p{};

    // SHA/SHX/SHY instructions need to know if there was a page cross while
    // getting the effective address.
    bool page_cross;

    std::array<u8, AddrSpaceSize> ram{0};

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

    // Helpers
    void Add(u8 data);
    void Branch(bool cond);
    void Compare(u8 left, u8 right);
    void ShHelper(u16 addr, u8 val);

    // Stack
    u8 Peek() const;
    u8 Pop();
    void Push(u8 data);
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
}

inline u8 Cpu::Peek() const {
    return Read(StackAddr + s);
}

inline u8 Cpu::Pop() {
    return Read(StackAddr + ++s);
}

inline void Cpu::Push(u8 data) {
    Write(StackAddr + s--, data);
}

}
