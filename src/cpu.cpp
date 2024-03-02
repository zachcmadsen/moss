#include "cpu.h"

#include <cstdint>

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
    pc = static_cast<std::uint16_t>(pcl | pch << 8);
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
    std::uint8_t low;
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

}
