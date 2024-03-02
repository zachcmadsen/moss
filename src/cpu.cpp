#include "cpu.h"

#include "integer.h"

namespace moss {

void Cpu::Reset() {
    Read(pc);
    Peek();
    --s;
    Peek();
    --s;
    Peek();
    --s;
    p.i = true;
    auto pcl = Read(0xFFFC);
    auto pch = Read(0xFFFC + 1);
    pc = u16(pcl | pch << 8);
}

void Cpu::Step() {
    auto opc = ram[pc++];
    // clang-format off
    switch (opc) {
        case 0x00: Brk(0); break;
        case 0x01: Ora(Idx()); break;
        case 0x03: Slo(Idx()); break;
        case 0x04: Nop(Zpg()); break;
        case 0x05: Ora(Zpg()); break;
        case 0x06: Asl(Zpg()); break;
        case 0x07: Slo(Zpg()); break;
        case 0x08: Php(0); break;
        case 0x09: Ora(Imm()); break;
        case 0x0A: AslA(0); break;
        case 0x0B: Anc(Imm()); break;
        case 0x0C: Nop(Abs()); break;
        case 0x0D: Ora(Abs()); break;
        case 0x0E: Asl(Abs()); break;
        case 0x0F: Slo(Abs()); break;
        case 0x10: Bpl(0); break;
        case 0x11: Ora(Idy<false>()); break;
        case 0x13: Slo(Idy<true>()); break;
        case 0x14: Nop(Zpx()); break;
        case 0x15: Ora(Zpx()); break;
        case 0x16: Asl(Zpx()); break;
        case 0x17: Slo(Zpx()); break;
        case 0x18: Clc(0); break;
        case 0x19: Ora(Aby<false>()); break;
        case 0x1A: Nop(Imp()); break;
        case 0x1B: Slo(Aby<true>()); break;
        case 0x1C: Nop(Abx<false>()); break;
        case 0x1D: Ora(Abx<false>()); break;
        case 0x1E: Asl(Abx<true>()); break;
        case 0x1F: Slo(Abx<true>()); break;
        case 0x20: Jsr(0); break;
        case 0x21: And(Idx()); break;
        case 0x23: Rla(Idx()); break;
        case 0x24: Bit(Zpg()); break;
        case 0x25: And(Zpg()); break;
        case 0x26: Rol(Zpg()); break;
        case 0x27: Rla(Zpg()); break;
        case 0x28: Plp(0); break;
        case 0x29: And(Imm()); break;
        case 0x2A: RolA(0); break;
        case 0x2B: Anc(Imm()); break;
        case 0x2C: Bit(Abs()); break;
        case 0x2D: And(Abs()); break;
        case 0x2E: Rol(Abs()); break;
        case 0x2F: Rla(Abs()); break;
        case 0x30: Bmi(0); break;
        case 0x31: And(Idy<false>()); break;
        case 0x33: Rla(Idy<true>()); break;
        case 0x34: Nop(Zpx()); break;
        case 0x35: And(Zpx()); break;
        case 0x36: Rol(Zpx()); break;
        case 0x37: Rla(Zpx()); break;
        case 0x38: Sec(0); break;
        case 0x39: And(Aby<false>()); break;
        case 0x3A: Nop(Imp()); break;
        case 0x3B: Rla(Aby<true>()); break;
        case 0x3C: Nop(Abx<false>()); break;
        case 0x3D: And(Abx<false>()); break;
        case 0x3E: Rol(Abx<true>()); break;
        case 0x3F: Rla(Abx<true>()); break;
        case 0x40: Rti(0); break;
        case 0x41: Eor(Idx()); break;
        case 0x43: Sre(Idx()); break;
        case 0x44: Nop(Zpg()); break;
        case 0x45: Eor(Zpg()); break;
        case 0x46: Lsr(Zpg()); break;
        case 0x47: Sre(Zpg()); break;
        case 0x48: Pha(0); break;
        case 0x49: Eor(Imm()); break;
        case 0x4A: LsrA(0); break;
        case 0x4B: Alr(Imm()); break;
        case 0x4C: Jmp(Abs()); break;
        case 0x4D: Eor(Abs()); break;
        case 0x4E: Lsr(Abs()); break;
        case 0x4F: Sre(Abs()); break;
        case 0x50: Bvc(0); break;
        case 0x51: Eor(Idy<false>()); break;
        case 0x53: Sre(Idy<true>()); break;
        case 0x54: Nop(Zpx()); break;
        case 0x55: Eor(Zpx()); break;
        case 0x56: Lsr(Zpx()); break;
        case 0x57: Sre(Zpx()); break;
        case 0x58: Cli(0); break;
        case 0x59: Eor(Aby<false>()); break;
        case 0x5A: Nop(Imp()); break;
        case 0x5B: Sre(Aby<true>()); break;
        case 0x5C: Nop(Abx<false>()); break;
        case 0x5D: Eor(Abx<false>()); break;
        case 0x5E: Lsr(Abx<true>()); break;
        case 0x5F: Sre(Abx<true>()); break;
        case 0x60: Rts(0); break;
        case 0x61: Adc(Idx()); break;
        case 0x63: Rra(Idx()); break;
        case 0x64: Nop(Zpg()); break;
        case 0x65: Adc(Zpg()); break;
        case 0x66: Ror(Zpg()); break;
        case 0x67: Rra(Zpg()); break;
        case 0x68: Pla(0); break;
        case 0x69: Adc(Imm()); break;
        case 0x6A: RorA(0); break;
        case 0x6B: Arr(Imm()); break;
        case 0x6C: Jmp(Ind()); break;
        case 0x6D: Adc(Abs()); break;
        case 0x6E: Ror(Abs()); break;
        case 0x6F: Rra(Abs()); break;
        case 0x70: Bvs(0); break;
        case 0x71: Adc(Idy<false>()); break;
        case 0x73: Rra(Idy<true>()); break;
        case 0x74: Nop(Zpx()); break;
        case 0x75: Adc(Zpx()); break;
        case 0x76: Ror(Zpx()); break;
        case 0x77: Rra(Zpx()); break;
        case 0x78: Sei(0); break;
        case 0x79: Adc(Aby<false>()); break;
        case 0x7A: Nop(Imp()); break;
        case 0x7B: Rra(Aby<true>()); break;
        case 0x7C: Nop(Abx<false>()); break;
        case 0x7D: Adc(Abx<false>()); break;
        case 0x7E: Ror(Abx<true>()); break;
        case 0x7F: Rra(Abx<true>()); break;
        case 0x80: Nop(Imm()); break;
        case 0x81: Sta(Idx()); break;
        case 0x82: Nop(Imm()); break;
        case 0x83: Sax(Idx()); break;
        case 0x84: Sty(Zpg()); break;
        case 0x85: Sta(Zpg()); break;
        case 0x86: Stx(Zpg()); break;
        case 0x87: Sax(Zpg()); break;
        case 0x88: Dey(0); break;
        case 0x89: Nop(Imm()); break;
        case 0x8A: Txa(0); break;
        case 0x8B: break; // ane
        case 0x8C: Sty(Abs()); break;
        case 0x8D: Sta(Abs()); break;
        case 0x8E: Stx(Abs()); break;
        case 0x8F: Sax(Abs()); break;
        case 0x90: Bcc(0); break;
        case 0x91: Sta(Idy<true>()); break;
        case 0x93: Sha(Idy<true>()); break;
        case 0x94: Sty(Zpx()); break;
        case 0x95: Sta(Zpx()); break;
        case 0x96: Stx(Zpy()); break;
        case 0x97: Sax(Zpy()); break;
        case 0x98: Tya(0); break;
        case 0x99: Sta(Aby<true>()); break;
        case 0x9A: Txs(0); break;
        case 0x9B: Tas(Aby<true>()); break;
        case 0x9C: Shy(Abx<true>()); break;
        case 0x9D: Sta(Abx<true>()); break;
        case 0x9E: Shx(Aby<true>()); break;
        case 0x9F: Sha(Aby<true>()); break;
        case 0xA0: Ldy(Imm()); break;
        case 0xA1: Lda(Idx()); break;
        case 0xA2: Ldx(Imm()); break;
        case 0xA3: Lax(Idx()); break;
        case 0xA4: Ldy(Zpg()); break;
        case 0xA5: Lda(Zpg()); break;
        case 0xA6: Ldx(Zpg()); break;
        case 0xA7: Lax(Zpg()); break;
        case 0xA8: Tay(0); break;
        case 0xA9: Lda(Imm()); break;
        case 0xAA: Tax(0); break;
        case 0xAB: break; //Lxa(Imm()); break;
        case 0xAC: Ldy(Abs()); break;
        case 0xAD: Lda(Abs()); break;
        case 0xAE: Ldx(Abs()); break;
        case 0xAF: Lax(Abs()); break;
        case 0xB0: Bcs(0); break;
        case 0xB1: Lda(Idy<false>()); break;
        case 0xB3: Lax(Idy<false>()); break;
        case 0xB4: Ldy(Zpx()); break;
        case 0xB5: Lda(Zpx()); break;
        case 0xB6: Ldx(Zpy()); break;
        case 0xB7: Lax(Zpy()); break;
        case 0xB8: Clv(0); break;
        case 0xB9: Lda(Aby<false>()); break;
        case 0xBA: Tsx(0); break;
        case 0xBB: Las(Aby<false>()); break;
        case 0xBC: Ldy(Abx<false>()); break;
        case 0xBD: Lda(Abx<false>()); break;
        case 0xBE: Ldx(Aby<false>()); break;
        case 0xBF: Lax(Aby<false>()); break;
        case 0xC0: Cpy(Imm()); break;
        case 0xC1: Cmp(Idx()); break;
        case 0xC2: Nop(Imm()); break;
        case 0xC3: Dcp(Idx()); break;
        case 0xC4: Cpy(Zpg()); break;
        case 0xC5: Cmp(Zpg()); break;
        case 0xC6: Dec(Zpg()); break;
        case 0xC7: Dcp(Zpg()); break;
        case 0xC8: Iny(0); break;
        case 0xC9: Cmp(Imm()); break;
        case 0xCA: Dex(0); break;
        case 0xCB: Sbx(Imm()); break;
        case 0xCC: Cpy(Abs()); break;
        case 0xCD: Cmp(Abs()); break;
        case 0xCE: Dec(Abs()); break;
        case 0xCF: Dcp(Abs()); break;
        case 0xD0: Bne(0); break;
        case 0xD1: Cmp(Idy<false>()); break;
        case 0xD3: Dcp(Idy<true>()); break;
        case 0xD4: Nop(Zpx()); break;
        case 0xD5: Cmp(Zpx()); break;
        case 0xD6: Dec(Zpx()); break;
        case 0xD7: Dcp(Zpx()); break;
        case 0xD8: Cld(0); break;
        case 0xD9: Cmp(Aby<false>()); break;
        case 0xDA: Nop(Imp()); break;
        case 0xDB: Dcp(Aby<true>()); break;
        case 0xDC: Nop(Abx<false>()); break;
        case 0xDD: Cmp(Abx<false>()); break;
        case 0xDE: Dec(Abx<true>()); break;
        case 0xDF: Dcp(Abx<true>()); break;
        case 0xE0: Cpx(Imm()); break;
        case 0xE1: Sbc(Idx()); break;
        case 0xE2: Nop(Imm()); break;
        case 0xE3: Isc(Idx()); break;
        case 0xE4: Cpx(Zpg()); break;
        case 0xE5: Sbc(Zpg()); break;
        case 0xE6: Inc(Zpg()); break;
        case 0xE7: Isc(Zpg()); break;
        case 0xE8: Inx(0); break;
        case 0xE9: Sbc(Imm()); break;
        case 0xEA: Nop(Imp()); break;
        case 0xEB: Sbc(Imm()); break;
        case 0xEC: Cpx(Abs()); break;
        case 0xED: Sbc(Abs()); break;
        case 0xEE: Inc(Abs()); break;
        case 0xEF: Isc(Abs()); break;
        case 0xF0: Beq(0); break;
        case 0xF1: Sbc(Idy<false>()); break;
        case 0xF3: Isc(Idy<true>()); break;
        case 0xF4: Nop(Zpx()); break;
        case 0xF5: Sbc(Zpx()); break;
        case 0xF6: Inc(Zpx()); break;
        case 0xF7: Isc(Zpx()); break;
        case 0xF8: Sed(0); break;
        case 0xF9: Sbc(Aby<false>()); break;
        case 0xFA: Nop(Imp()); break;
        case 0xFB: Isc(Aby<true>()); break;
        case 0xFC: Nop(Abx<false>()); break;
        case 0xFD: Sbc(Abx<false>()); break;
        case 0xFE: Inc(Abx<true>()); break;
        case 0xFF: Isc(Abx<true>()); break;
    }
    // clang-format on
}

u16 Cpu::Abs() {
    auto low = Read(pc++);
    auto high = Read(pc++);
    return u16(low | high << 8);
}

template <bool write> u16 Cpu::Abx() {
    u8 low;
    page_cross = __builtin_add_overflow(Read(pc++), x, &low);
    u8 high = Read(pc++);
    if constexpr (write) {
        Read(u16(low | high << 8));
    } else if (page_cross) {
        Read(u16(low | high << 8));
    }
    return u16(low | (high + page_cross) << 8);
}

template <bool write> u16 Cpu::Aby() {
    u8 low;
    page_cross = __builtin_add_overflow(Read(pc++), y, &low);
    auto high = Read(pc++);
    if constexpr (write) {
        Read(u16(low | high << 8));
    } else if (page_cross) {
        Read(u16(low | high << 8));
    }
    return u16(low | (high + page_cross) << 8);
}

u16 Cpu::Idx() {
    auto ptr = Read(pc++);
    Read(ptr);
    ptr += x;
    auto low = Read(ptr);
    auto high = Read(u8(ptr + 1));
    return u16(low | high << 8);
}

template <bool write> u16 Cpu::Idy() {
    auto ptr = Read(pc++);
    u8 low;
    page_cross = __builtin_add_overflow(Read(ptr), y, &low);
    auto high = Read(u8(ptr + 1));
    if constexpr (write) {
        Read(u16(low | high << 8));
    } else if (page_cross) {
        Read(u16(low | high << 8));
    }
    return u16(low | (high + page_cross) << 8);
}

u16 Cpu::Imm() {
    return pc++;
};

u16 Cpu::Imp() {
    return pc;
}

u16 Cpu::Ind() {
    auto ptr_low = Read(pc++);
    auto ptr_high = Read(pc++);
    auto low = Read(u16(ptr_low | ptr_high << 8));
    auto high = Read(u16(u8(ptr_low + 1) | ptr_high << 8));
    return u16(low | high << 8);
}

u16 Cpu::Zpg() {
    return Read(pc++);
}

u16 Cpu::Zpx() {
    auto addr = Read(pc++);
    Read(addr);
    return u8(addr + x);
}

u16 Cpu::Zpy() {
    auto addr = Read(pc++);
    Read(addr);
    return u8(addr + y);
}

void Cpu::Adc(u16 addr) {
    auto data = Read(addr);
    Add(data);
};

void Cpu::Alr(u16 addr) {
    a &= Read(addr);
    bool carry = a & 0x01;
    a >>= 1;
    p.c = carry;
    p.z = !a;
    p.n = a & 0x80;
}

void Cpu::Anc(u16 addr) {
    a &= Read(addr);
    p.c = a & 0x80;
    p.z = !a;
    p.n = a & 0x80;
}

void Cpu::And(u16 addr) {
    a &= Read(addr);
    p.z = !a;
    p.n = a & 0x80;
}

void Cpu::Arr(u16 addr) {
    a &= Read(addr);
    a = u8(a >> 1 | p.c << 7);
    p.c = a & (1 << 6);
    p.v = (p.c << 5) ^ (a & (1 << 5));
    p.z = !a;
    p.n = a & 0x80;
}

void Cpu::Asl(u16 addr) {
    auto data = Read(addr);
    Write(addr, data);
    bool carry = data & 0x80;
    data <<= 1;
    Write(addr, data);
    p.c = carry;
    p.z = !data;
    p.n = data & 0x80;
}

void Cpu::AslA([[maybe_unused]] u16 addr) {
    Read(pc);
    bool carry = a & 0x80;
    a <<= 1;
    p.c = carry;
    p.z = !a;
    p.n = a & 0x80;
}

void Cpu::Bcc([[maybe_unused]] u16 addr) {
    Branch(!p.c);
}

void Cpu::Bcs([[maybe_unused]] u16 addr) {
    Branch(p.c);
}

void Cpu::Beq([[maybe_unused]] u16 addr) {
    Branch(p.z);
}

void Cpu::Bit(u16 addr) {
    auto data = Read(addr);
    p.z = !(a & data);
    p.v = data & 0x40;
    p.n = data & 0x80;
}

void Cpu::Bmi([[maybe_unused]] u16 addr) {
    Branch(p.n);
}

void Cpu::Bne([[maybe_unused]] u16 addr) {
    Branch(!p.z);
}

void Cpu::Bpl([[maybe_unused]] u16 addr) {
    Branch(!p.n);
}

void Cpu::Brk([[maybe_unused]] u16 addr) {
    Read(pc++);
    Push(pc >> 8);
    Push(u8(pc));
    auto p_with_b = p;
    p_with_b.b = true;
    Push(u8(p_with_b));
    p.i = true;
    auto pcl = Read(0xFFFE);
    auto pch = Read(0xFFFE + 1);
    pc = u16(pcl | pch << 8);
}

void Cpu::Bvc([[maybe_unused]] u16 addr) {
    Branch(!p.v);
}

void Cpu::Bvs([[maybe_unused]] u16 addr) {
    Branch(p.v);
}

void Cpu::Clc([[maybe_unused]] u16 addr) {
    Read(pc);
    p.c = false;
}

void Cpu::Cld([[maybe_unused]] u16 addr) {
    Read(pc);
    p.d = false;
}

void Cpu::Cli([[maybe_unused]] u16 addr) {
    Read(pc);
    p.i = false;
}

void Cpu::Clv([[maybe_unused]] u16 addr) {
    Read(pc);
    p.v = false;
}

void Cpu::Cmp(u16 addr) {
    auto data = Read(addr);
    Compare(a, data);
}

void Cpu::Cpx(u16 addr) {
    auto data = Read(addr);
    Compare(x, data);
}

void Cpu::Cpy(u16 addr) {
    auto data = Read(addr);
    Compare(y, data);
}

void Cpu::Dcp(u16 addr) {
    auto data = Read(addr);
    Write(addr, data);
    --data;
    Write(addr, data);
    Compare(a, data);
}

void Cpu::Dec(u16 addr) {
    auto data = Read(addr);
    Write(addr, data);
    --data;
    Write(addr, data);
    p.z = !data;
    p.n = data & 0x80;
}

void Cpu::Dex([[maybe_unused]] u16 addr) {
    Read(pc);
    --x;
    p.z = !x;
    p.n = x & 0x80;
}

void Cpu::Dey([[maybe_unused]] u16 addr) {
    Read(pc);
    --y;
    p.z = !y;
    p.n = y & 0x80;
}

void Cpu::Eor(u16 addr) {
    a ^= Read(addr);
    p.z = !a;
    p.n = a & 0x80;
};

void Cpu::Inc(u16 addr) {
    auto data = Read(addr);
    Write(addr, data);
    ++data;
    Write(addr, data);
    p.z = !data;
    p.n = data & 0x80;
}

void Cpu::Inx([[maybe_unused]] u16 addr) {
    Read(pc);
    ++x;
    p.z = !x;
    p.n = x & 0x80;
}

void Cpu::Iny([[maybe_unused]] u16 addr) {
    Read(pc);
    ++y;
    p.z = !y;
    p.n = y & 0x80;
}

void Cpu::Isc(u16 addr) {
    auto data = Read(addr);
    Write(addr, data);
    ++data;
    Write(addr, data);
    Add(data ^ 0xFF);
}

void Cpu::Jmp(u16 addr) {
    pc = addr;
}

void Cpu::Jsr([[maybe_unused]] u16 addr) {
    auto pcl = Read(pc++);
    Peek();
    Push(pc >> 8);
    Push(u8(pc));
    auto pch = Read(pc++);
    pc = u16(pcl | pch << 8);
}

void Cpu::Las(u16 addr) {
    a = Read(addr) & s;
    x = a;
    s = a;
    p.z = !a;
    p.n = a & 0x80;
}

void Cpu::Lax(u16 addr) {
    a = Read(addr);
    x = a;
    p.z = !a;
    p.n = a & 0x80;
}

void Cpu::Lda(u16 addr) {
    a = Read(addr);
    p.z = !a;
    p.n = a & 0x80;
};

void Cpu::Ldx(u16 addr) {
    x = Read(addr);
    p.z = !x;
    p.n = x & 0x80;
};

void Cpu::Ldy(u16 addr) {
    y = Read(addr);
    p.z = !y;
    p.n = y & 0x80;
};

void Cpu::Lsr(u16 addr) {
    auto data = Read(addr);
    Write(addr, data);
    bool carry = data & 0x01;
    data >>= 1;
    Write(addr, data);
    p.c = carry;
    p.z = !data;
    p.n = data & 0x80;
}

void Cpu::LsrA([[maybe_unused]] u16 addr) {
    Read(pc);
    bool carry = a & 0x01;
    a >>= 1;
    p.c = carry;
    p.z = !a;
    p.n = a & 0x80;
}

void Cpu::Nop(u16 addr) {
    Read(addr);
}

void Cpu::Ora(u16 addr) {
    a |= Read(addr);
    p.z = !a;
    p.n = a & 0x80;
};

void Cpu::Pha([[maybe_unused]] u16 addr) {
    Read(pc);
    Push(a);
}

void Cpu::Php([[maybe_unused]] u16 addr) {
    Read(pc);
    auto p_with_b_with_u = p;
    p_with_b_with_u.b = true;
    p_with_b_with_u.u = true;
    Push(u8(p_with_b_with_u));
}

void Cpu::Pla([[maybe_unused]] u16 addr) {
    Read(pc);
    Peek();
    a = Pop();
    p.z = !a;
    p.n = a & 0x80;
}

void Cpu::Plp([[maybe_unused]] u16 addr) {
    Read(pc);
    Peek();
    Status new_p(Pop());
    new_p.b = p.b;
    new_p.u = p.u;
    p = new_p;
}

void Cpu::Rla(u16 addr) {
    auto data = Read(addr);
    Write(addr, data);
    bool carry = data & 0x80;
    data = u8(p.c | data << 1);
    Write(addr, data);
    a &= data;
    p.c = carry;
    p.z = !a;
    p.n = a & 0x80;
}

void Cpu::Rol(u16 addr) {
    auto data = Read(addr);
    Write(addr, data);
    bool carry = data & 0x80;
    data = u8(p.c | data << 1);
    Write(addr, data);
    p.c = carry;
    p.z = !data;
    p.n = data & 0x80;
}

void Cpu::RolA([[maybe_unused]] u16 addr) {
    Read(pc);
    bool carry = a & 0x80;
    a = u8(p.c | a << 1);
    p.c = carry;
    p.z = !a;
    p.n = a & 0x80;
}

void Cpu::Ror(u16 addr) {
    auto data = Read(addr);
    Write(addr, data);
    bool carry = data & 0x01;
    data = u8(data >> 1 | p.c << 7);
    Write(addr, data);
    p.c = carry;
    p.z = !data;
    p.n = data & 0x80;
}

void Cpu::RorA([[maybe_unused]] u16 addr) {
    Read(pc);
    bool carry = a & 0x01;
    a = u8(a >> 1 | p.c << 7);
    p.c = carry;
    p.z = !a;
    p.n = a & 0x80;
}

void Cpu::Rra(u16 addr) {
    auto data = Read(addr);
    Write(addr, data);
    bool carry = data & 0x01;
    data = u8(data >> 1 | p.c << 7);
    Write(addr, data);
    p.c = carry;
    Add(data);
}

void Cpu::Rti([[maybe_unused]] u16 addr) {
    Read(pc);
    Peek();
    Status new_p(Pop());
    new_p.b = p.b;
    new_p.u = p.u;
    p = new_p;
    auto pcl = Pop();
    auto pch = Pop();
    pc = u16(pcl | pch << 8);
}

void Cpu::Rts([[maybe_unused]] u16 addr) {
    Read(pc);
    Peek();
    auto pcl = Pop();
    auto pch = Pop();
    pc = u16(pcl | pch << 8);
    Read(pc++);
}

void Cpu::Sax(u16 addr) {
    Write(addr, a & x);
}

void Cpu::Sbc(u16 addr) {
    auto data = Read(addr);
    Add(data ^ 0xFF);
}

void Cpu::Sbx(u16 addr) {
    auto data = Read(addr);
    u8 diff;
    bool overflow = __builtin_sub_overflow(a & x, data, &diff);
    x = diff;
    p.c = !overflow;
    p.z = !x;
    p.n = x & 0x80;
}

void Cpu::Sec([[maybe_unused]] u16 addr) {
    Read(pc);
    p.c = true;
}

void Cpu::Sed([[maybe_unused]] u16 addr) {
    Read(pc);
    p.d = true;
}

void Cpu::Sei([[maybe_unused]] u16 addr) {
    Read(pc);
    p.i = true;
}

void Cpu::Sha(u16 addr) {
    ShHelper(addr, a & x);
}

void Cpu::Shx(u16 addr) {
    ShHelper(addr, x);
}

void Cpu::Shy(u16 addr) {
    ShHelper(addr, y);
}

void Cpu::Slo(u16 addr) {
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

void Cpu::Sre(u16 addr) {
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

void Cpu::Sta(u16 addr) {
    Write(addr, a);
}

void Cpu::Stx(u16 addr) {
    Write(addr, x);
}

void Cpu::Sty(u16 addr) {
    Write(addr, y);
}

void Cpu::Tas(u16 addr) {
    s = a & x;
    ShHelper(addr, s);
}

void Cpu::Tax([[maybe_unused]] u16 addr) {
    Read(pc);
    x = a;
    p.z = !x;
    p.n = x & 0x80;
}

void Cpu::Tay([[maybe_unused]] u16 addr) {
    Read(pc);
    y = a;
    p.z = !y;
    p.n = y & 0x80;
}

void Cpu::Tsx([[maybe_unused]] u16 addr) {
    Read(pc);
    x = s;
    p.z = !x;
    p.n = x & 0x80;
}

void Cpu::Txa([[maybe_unused]] u16 addr) {
    Read(pc);
    a = x;
    p.z = !a;
    p.n = a & 0x80;
}

void Cpu::Txs([[maybe_unused]] u16 addr) {
    Read(pc);
    s = x;
}

void Cpu::Tya([[maybe_unused]] u16 addr) {
    Read(pc);
    a = y;
    p.z = !a;
    p.n = a & 0x80;
}

void Cpu::Add(u8 data) {
    auto prev_a = a;
    auto sum = a + data + p.c;
    a = u8(sum);
    p.c = sum > 0xFF;
    p.v = (prev_a ^ a) & (data ^ a) & 0x80;
    p.z = !a;
    p.n = a & 0x80;
}

void Cpu::Branch(bool cond) {
    auto offset = Read(pc++);
    if (cond) {
        Read(pc);

        auto prev_pc = pc;
        pc += i8(offset);

        if ((prev_pc & 0xFF00) != (pc & 0xFF00)) {
            Read(u8(prev_pc + offset) | (prev_pc & 0xFF00));
        }
    }
}

void Cpu::Compare(u8 left, u8 right) {
    u8 diff;
    bool overflow = __builtin_sub_overflow(left, right, &diff);
    p.c = !overflow;
    p.z = !diff;
    p.n = diff & 0x80;
}

// See https://github.com/TomHarte/ProcessorTests/issues/61.
void Cpu::ShHelper(u16 addr, u8 val) {
    u8 high = addr >> 8;
    u8 data = val & (high + !page_cross);
    high = page_cross ? data : high;
    Write(u16((addr & 0x00FF) | high << 8), data);
}

}
