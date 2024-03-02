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
    bool pageCross;

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
        std::uint8_t thing = reg & (high + !pageCross);
        high = pageCross ? thing : high;
        Write(static_cast<std::uint16_t>((addr & 0x00FF) | high << 8), thing);
    }

    std::uint16_t Abs() {
        auto low = Read(pc++);
        auto high = Read(pc++);
        return static_cast<std::uint16_t>(low | high << 8);
    }

    template <bool write> std::uint16_t Abx() {
        std::uint8_t low;
        pageCross = __builtin_add_overflow(Read(pc++), x, &low);
        std::uint8_t high = Read(pc++);

        if constexpr (write) {
            Read(static_cast<std::uint16_t>(low | high << 8));
        } else if (pageCross) {
            Read(static_cast<std::uint16_t>(low | high << 8));
        }

        return static_cast<std::uint16_t>(low | (high + pageCross) << 8);
    }

    template <bool write> std::uint16_t Aby() {
        std::uint8_t low;
        pageCross = __builtin_add_overflow(Read(pc++), y, &low);
        auto high = Read(pc++);

        if constexpr (write) {
            Read(static_cast<uint16_t>(low | high << 8));
        } else if (pageCross) {
            Read(static_cast<uint16_t>(low | high << 8));
        }

        return static_cast<uint16_t>(low | (high + pageCross) << 8);
    }

    std::uint16_t Imm() {
        return pc++;
    };

    std::uint16_t Imp() {
        return pc;
    }

    std::uint16_t Ind() {
        auto ptr_low = Read(pc++);
        auto ptr_high = Read(pc++);
        auto low = Read(static_cast<uint16_t>(ptr_low | ptr_high << 8));
        auto high = Read(static_cast<uint16_t>(
            static_cast<std::uint8_t>(ptr_low + 1) | ptr_high << 8));
        return static_cast<uint16_t>(low | high << 8);
    }

    std::uint16_t Idx() {
        auto ptr = Read(pc++);
        Read(ptr);
        ptr += x;
        auto low = Read(ptr);
        auto high = Read(static_cast<std::uint8_t>(ptr + 1));
        return static_cast<uint16_t>(low | high << 8);
    }

    template <bool write> std::uint16_t Idy() {
        auto ptr = Read(pc++);
        std::uint8_t low;
        pageCross = __builtin_add_overflow(Read(ptr), y, &low);
        auto high = Read(static_cast<std::uint8_t>(ptr + 1));

        if constexpr (write) {
            Read(static_cast<uint16_t>(low | high << 8));
        } else if (pageCross) {
            Read(static_cast<uint16_t>(low | high << 8));
        }

        return static_cast<uint16_t>(low | (high + pageCross) << 8);
    }

    std::uint16_t Zpg() {
        return Read(pc++);
    }

    std::uint16_t Zpx() {
        auto addr = Read(pc++);
        Read(addr);
        return static_cast<std::uint8_t>(addr + x);
    }

    std::uint16_t Zpy() {
        auto addr = Read(pc++);
        Read(addr);
        return static_cast<std::uint8_t>(addr + y);
    }

    void Adc(std::uint16_t addr) {
        auto data = Read(addr);
        Add(data);
    };

    void Alr(std::uint16_t addr) {
        a &= Read(addr);
        bool carry = a & 0x01;
        a >>= 1;
        p.c = carry;
        p.z = !a;
        p.n = a & 0x80;
    }

    void Anc(std::uint16_t addr) {
        a &= Read(addr);
        p.c = a & 0x80;
        p.z = !a;
        p.n = a & 0x80;
    }

    void And(std::uint16_t addr) {
        a &= Read(addr);
        p.z = !a;
        p.n = a & 0x80;
    }

    void Arr(std::uint16_t addr) {
        a &= Read(addr);
        a = static_cast<std::uint8_t>(a >> 1 | p.c << 7);
        p.c = a & (1 << 6);
        p.v = (p.c << 5) ^ (a & (1 << 5));
        p.z = !a;
        p.n = a & 0x80;
    }

    void Asl(std::uint16_t addr) {
        auto data = Read(addr);
        Write(addr, data);
        bool carry = data & 0x80;
        data <<= 1;
        Write(addr, data);
        p.c = carry;
        p.z = !data;
        p.n = data & 0x80;
    }

    void AslA([[maybe_unused]] std::uint16_t addr) {
        Read(pc);
        bool carry = a & 0x80;
        a <<= 1;
        p.c = carry;
        p.z = !a;
        p.n = a & 0x80;
    }

    void Bcc([[maybe_unused]] std::uint16_t addr) {
        Branch(!p.c);
    }

    void Bcs([[maybe_unused]] std::uint16_t addr) {
        Branch(p.c);
    }

    void Beq([[maybe_unused]] std::uint16_t addr) {
        Branch(p.z);
    }

    void Bit(std::uint16_t addr) {
        auto data = Read(addr);
        p.z = !(a & data);
        p.v = data & 0x40;
        p.n = data & 0x80;
    }

    void Bmi([[maybe_unused]] std::uint16_t addr) {
        Branch(p.n);
    }

    void Bne([[maybe_unused]] std::uint16_t addr) {
        Branch(!p.z);
    }

    void Bpl([[maybe_unused]] std::uint16_t addr) {
        Branch(!p.n);
    }

    void Brk([[maybe_unused]] std::uint16_t addr) {
        Read(pc++);
        Push(pc >> 8);
        Push(static_cast<std::uint8_t>(pc));
        auto p_with_b = p;
        p_with_b.b = true;
        Push(u8(p_with_b));
        p.i = true;
        auto pcl = Read(0xFFFE);
        auto pch = Read(0xFFFE + 1);
        pc = static_cast<std::uint16_t>(pcl | pch << 8);
    }

    void Bvc([[maybe_unused]] std::uint16_t addr) {
        Branch(!p.v);
    }

    void Bvs([[maybe_unused]] std::uint16_t addr) {
        Branch(p.v);
    }

    void Clc([[maybe_unused]] std::uint16_t addr) {
        Read(pc);
        p.c = false;
    }

    void Cld([[maybe_unused]] std::uint16_t addr) {
        Read(pc);
        p.d = false;
    }

    void Cli([[maybe_unused]] std::uint16_t addr) {
        Read(pc);
        p.i = false;
    }

    void Clv([[maybe_unused]] std::uint16_t addr) {
        Read(pc);
        p.v = false;
    }

    void Cmp(std::uint16_t addr) {
        auto data = Read(addr);
        Compare(a, data);
    }

    void Cpx(std::uint16_t addr) {
        auto data = Read(addr);
        Compare(x, data);
    }

    void Cpy(std::uint16_t addr) {
        auto data = Read(addr);
        Compare(y, data);
    }

    void Dcp(std::uint16_t addr) {
        auto data = Read(addr);
        Write(addr, data);
        --data;
        Write(addr, data);
        Compare(a, data);
    }

    void Dec(std::uint16_t addr) {
        auto data = Read(addr);
        Write(addr, data);
        --data;
        Write(addr, data);
        p.z = !data;
        p.n = data & 0x80;
    }

    void Dex([[maybe_unused]] std::uint16_t addr) {
        Read(pc);
        --x;
        p.z = !x;
        p.n = x & 0x80;
    }

    void Dey([[maybe_unused]] std::uint16_t addr) {
        Read(pc);
        --y;
        p.z = !y;
        p.n = y & 0x80;
    }

    void Eor(std::uint16_t addr) {
        a ^= Read(addr);
        p.z = !a;
        p.n = a & 0x80;
    };

    void Inc(std::uint16_t addr) {
        auto data = Read(addr);
        Write(addr, data);
        ++data;
        Write(addr, data);
        p.z = !data;
        p.n = data & 0x80;
    }

    void Inx([[maybe_unused]] std::uint16_t addr) {
        Read(pc);
        ++x;
        p.z = !x;
        p.n = x & 0x80;
    }

    void Iny([[maybe_unused]] std::uint16_t addr) {
        Read(pc);
        ++y;
        p.z = !y;
        p.n = y & 0x80;
    }

    void Isc(std::uint16_t addr) {
        auto data = Read(addr);
        Write(addr, data);
        ++data;
        Write(addr, data);
        Add(data ^ 0xFF);
    }

    void Jmp(std::uint16_t addr) {
        pc = addr;
    }

    void Jsr([[maybe_unused]] std::uint16_t addr) {
        auto pcl = Read(pc++);
        Peek();
        Push(pc >> 8);
        Push(static_cast<std::uint8_t>(pc));
        auto pch = Read(pc++);
        pc = static_cast<std::uint16_t>(pcl | pch << 8);
    }

    void Las(std::uint16_t addr) {
        a = Read(addr) & s;
        x = a;
        s = a;
        p.z = !a;
        p.n = a & 0x80;
    }

    void Lax(std::uint16_t addr) {
        a = Read(addr);
        x = a;
        p.z = !a;
        p.n = a & 0x80;
    }

    void Lda(std::uint16_t addr) {
        a = Read(addr);
        p.z = !a;
        p.n = a & 0x80;
    };

    void Ldx(std::uint16_t addr) {
        x = Read(addr);
        p.z = !x;
        p.n = x & 0x80;
    };

    void Ldy(std::uint16_t addr) {
        y = Read(addr);
        p.z = !y;
        p.n = y & 0x80;
    };

    void Lsr(std::uint16_t addr) {
        auto data = Read(addr);
        Write(addr, data);
        bool carry = data & 0x01;
        data >>= 1;
        Write(addr, data);
        p.c = carry;
        p.z = !data;
        p.n = data & 0x80;
    }

    void LsrA([[maybe_unused]] std::uint16_t addr) {
        Read(pc);
        bool carry = a & 0x01;
        a >>= 1;
        p.c = carry;
        p.z = !a;
        p.n = a & 0x80;
    }

    void Nop(std::uint16_t addr) {
        Read(addr);
    }

    void Ora(std::uint16_t addr) {
        a |= Read(addr);
        p.z = !a;
        p.n = a & 0x80;
    };

    void Pha([[maybe_unused]] std::uint16_t addr) {
        Read(pc);
        Push(a);
    }

    void Php([[maybe_unused]] std::uint16_t addr) {
        Read(pc);
        auto p_with_b_with_u = p;
        p_with_b_with_u.b = true;
        p_with_b_with_u.u = true;
        Push(u8(p_with_b_with_u));
    }

    void Pla([[maybe_unused]] std::uint16_t addr) {
        Read(pc);
        Peek();
        a = Pop();
        p.z = !a;
        p.n = a & 0x80;
    }

    void Plp([[maybe_unused]] std::uint16_t addr) {
        Read(pc);
        Peek();
        Status new_p(Pop());
        new_p.b = p.b;
        new_p.u = p.u;
        p = new_p;
    }

    void Rla(std::uint16_t addr) {
        auto data = Read(addr);
        Write(addr, data);
        bool carry = data & 0x80;
        data = static_cast<std::uint8_t>(p.c | data << 1);
        Write(addr, data);
        a &= data;
        p.c = carry;
        p.z = !a;
        p.n = a & 0x80;
    }

    void Rol(std::uint16_t addr) {
        auto data = Read(addr);
        Write(addr, data);
        bool carry = data & 0x80;
        data = static_cast<std::uint8_t>(p.c | data << 1);
        Write(addr, data);
        p.c = carry;
        p.z = !data;
        p.n = data & 0x80;
    }

    void RolA([[maybe_unused]] std::uint16_t addr) {
        Read(pc);
        bool carry = a & 0x80;
        a = static_cast<std::uint8_t>(p.c | a << 1);
        p.c = carry;
        p.z = !a;
        p.n = a & 0x80;
    }

    void Ror(std::uint16_t addr) {
        auto data = Read(addr);
        Write(addr, data);
        bool carry = data & 0x01;
        data = static_cast<std::uint8_t>(data >> 1 | p.c << 7);
        Write(addr, data);
        p.c = carry;
        p.z = !data;
        p.n = data & 0x80;
    }

    void RorA([[maybe_unused]] std::uint16_t addr) {
        Read(pc);
        bool carry = a & 0x01;
        a = static_cast<std::uint8_t>(a >> 1 | p.c << 7);
        p.c = carry;
        p.z = !a;
        p.n = a & 0x80;
    }

    void Rra(std::uint16_t addr) {
        auto data = Read(addr);
        Write(addr, data);
        bool carry = data & 0x01;
        data = static_cast<std::uint8_t>(data >> 1 | p.c << 7);
        Write(addr, data);
        p.c = carry;
        Add(data);
    }

    void Rti([[maybe_unused]] std::uint16_t addr) {
        Read(pc);
        Peek();
        Status new_p(Pop());
        new_p.b = p.b;
        new_p.u = p.u;
        p = new_p;
        auto pcl = Pop();
        auto pch = Pop();
        pc = static_cast<std::uint16_t>(pcl | pch << 8);
    }

    void Rts([[maybe_unused]] std::uint16_t addr) {
        Read(pc);
        Peek();
        auto pcl = Pop();
        auto pch = Pop();
        pc = static_cast<std::uint16_t>(pcl | pch << 8);
        Read(pc++);
    }

    void Sax(std::uint16_t addr) {
        Write(addr, a & x);
    }

    void Sbc(std::uint16_t addr) {
        auto data = Read(addr);
        Add(data ^ 0xFF);
    }

    void Sbx(std::uint16_t addr) {
        auto data = Read(addr);
        std::uint8_t diff;
        bool overflow = __builtin_sub_overflow(a & x, data, &diff);
        x = diff;
        p.c = !overflow;
        p.z = !x;
        p.n = x & 0x80;
    }

    void Sec([[maybe_unused]] std::uint16_t addr) {
        Read(pc);
        p.c = true;
    }

    void Sed([[maybe_unused]] std::uint16_t addr) {
        Read(pc);
        p.d = true;
    }

    void Sei([[maybe_unused]] std::uint16_t addr) {
        Read(pc);
        p.i = true;
    }

    void Sha(std::uint16_t addr) {
        ShInner(addr, a & x);
    }

    void Shx(std::uint16_t addr) {
        ShInner(addr, x);
    }

    void Shy(std::uint16_t addr) {
        ShInner(addr, y);
    }

    void Slo(std::uint16_t addr) {
        auto data = Read(addr);
        Write(addr, data);
        bool carry = data & 0x80;
        data <<= 1;
        Write(addr, data);
        a |= data;
        p.c = carry;
        p.z = !a;
        p.n = a & 0x80;
    }

    void Sre(std::uint16_t addr) {
        auto data = Read(addr);
        Write(addr, data);
        bool carry = data & 0x01;
        data >>= 1;
        Write(addr, data);
        a ^= data;
        p.c = carry;
        p.z = !a;
        p.n = a & 0x80;
    }

    void Sta(std::uint16_t addr) {
        Write(addr, a);
    }

    void Stx(std::uint16_t addr) {
        Write(addr, x);
    }

    void Sty(std::uint16_t addr) {
        Write(addr, y);
    }

    void Tas(std::uint16_t addr) {
        s = a & x;
        ShInner(addr, s);
    }

    void Tax([[maybe_unused]] std::uint16_t addr) {
        Read(pc);
        x = a;
        p.z = !x;
        p.n = x & 0x80;
    }

    void Tay([[maybe_unused]] std::uint16_t addr) {
        Read(pc);
        y = a;
        p.z = !y;
        p.n = y & 0x80;
    }

    void Tsx([[maybe_unused]] std::uint16_t addr) {
        Read(pc);
        x = s;
        p.z = !x;
        p.n = x & 0x80;
    }

    void Txa([[maybe_unused]] std::uint16_t addr) {
        Read(pc);
        a = x;
        p.z = !a;
        p.n = a & 0x80;
    }

    void Txs([[maybe_unused]] std::uint16_t addr) {
        Read(pc);
        s = x;
    }

    void Tya([[maybe_unused]] std::uint16_t addr) {
        Read(pc);
        a = y;
        p.z = !a;
        p.n = a & 0x80;
    }
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
