#include <iso646.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cpu.h"

#define SET_ZN(reg)                                                            \
    do {                                                                       \
        cpu->p.z = (reg) == 0;                                                 \
        cpu->p.n = ((reg) & 0x80) != 0;                                        \
    } while (0)

enum { ADDR_SPACE_SIZE = 65536 };

static const int STACK_BASE_ADDR = 0x0100;

struct status {
    bool c : 1;
    bool z : 1;
    bool i : 1;
    bool d : 1;
    bool b : 1;
    bool u : 1;
    bool v : 1;
    bool n : 1;
};
static_assert(sizeof(struct status) == 1);

struct cpu {
    uint16_t pc;
    uint8_t a;
    uint8_t x;
    uint8_t y;
    uint8_t s;
    struct status p;

    uint16_t addr;

    uint8_t ram[ADDR_SPACE_SIZE];

#ifdef PROCESSOR_TESTS
    struct {
        uint16_t addr;
        uint8_t data;
    } bus_states[7];
    size_t cycles;
#endif
};

//-----------------------------------------------------------------------------
// Utils
//-----------------------------------------------------------------------------

static uint8_t read_byte(struct cpu *cpu, uint16_t addr) {
#ifdef PROCESSOR_TESTS
    uint8_t data = cpu->ram[addr];
    cpu->bus_states[cpu->cycles].addr = addr;
    cpu->bus_states[cpu->cycles].data = data;
    ++cpu->cycles;
    return data;
#else
    return cpu->ram[addr];
#endif
}

static void write_byte(struct cpu *cpu, uint16_t addr, uint8_t data) {
#ifdef PROCESSOR_TESTS
    cpu->ram[addr] = data;
    cpu->bus_states[cpu->cycles].addr = addr;
    cpu->bus_states[cpu->cycles].data = data;
    ++cpu->cycles;
#else
    cpu->ram[addr] = data;
#endif
}

static void add(struct cpu *cpu, uint8_t data) {
    uint8_t prev_a = cpu->a;
    uint16_t sum = cpu->a + data + cpu->p.c;
    cpu->a = sum;
    cpu->p.c = sum > 0xFF;
    cpu->p.v = (prev_a ^ cpu->a) & (data ^ cpu->a) & 0x80;
    SET_ZN(cpu->a);
}

static void branch(struct cpu *cpu, bool cond) {
    uint8_t offset = read_byte(cpu, cpu->pc++);
    if (cond) {
        read_byte(cpu, cpu->pc);

        uint16_t prev_pc = cpu->pc;
        cpu->pc += (int8_t)offset;

        if ((prev_pc & 0xFF00) != (cpu->pc & 0xFF00)) {
            read_byte(cpu, (uint8_t)(prev_pc + offset) | (prev_pc & 0xFF00));
        }
    }
}

static void compare(struct cpu *cpu, uint8_t reg, uint8_t data) {
    uint8_t res = reg - data;
    bool carry = res > reg;
    cpu->p.c = carry;
    SET_ZN(res);
}

#define PEEK()                                                                 \
    do {                                                                       \
        read_byte(cpu, STACK_BASE_ADDR + cpu->s);                              \
    } while (0)

#define PUSH(data)                                                             \
    do {                                                                       \
        write_byte(cpu, STACK_BASE_ADDR + cpu->s, (data));                     \
        --cpu->s;                                                              \
    } while (0)

//-----------------------------------------------------------------------------
// Addressing modes
//-----------------------------------------------------------------------------

static void abt(struct cpu *cpu) {
    uint8_t low = read_byte(cpu, cpu->pc++);
    uint8_t high = read_byte(cpu, cpu->pc++);
    cpu->addr = low | high << 8;
}

static void abx_r(struct cpu *cpu) {
    uint8_t low = read_byte(cpu, cpu->pc++) + cpu->x;
    bool overflow = low < cpu->x;
    uint8_t high = read_byte(cpu, cpu->pc++);
    if (overflow) {
        read_byte(cpu, low | high << 8);
    }
    cpu->addr = low | (high + overflow) << 8;
}

static void aby_r(struct cpu *cpu) {
    uint8_t low = read_byte(cpu, cpu->pc++) + cpu->y;
    bool overflow = low < cpu->y;
    uint8_t high = read_byte(cpu, cpu->pc++);
    if (overflow) {
        read_byte(cpu, low | high << 8);
    }
    cpu->addr = low | (high + overflow) << 8;
}

static void idx(struct cpu *cpu) {
    uint8_t ptr = read_byte(cpu, cpu->pc++);
    read_byte(cpu, ptr);
    ptr += cpu->x;
    uint8_t low = read_byte(cpu, ptr);
    uint8_t high = read_byte(cpu, (uint8_t)(ptr + 1));
    cpu->addr = low | high << 8;
}

static void idy(struct cpu *cpu) {
    uint8_t ptr = read_byte(cpu, cpu->pc++);
    uint8_t low = read_byte(cpu, ptr) + cpu->y;
    bool overflow = low < cpu->y;
    uint8_t high = read_byte(cpu, (uint8_t)(ptr + 1));
    if (overflow) {
        read_byte(cpu, low | high << 8);
    }
    cpu->addr = low | (high + overflow) << 8;
}

static void imm(struct cpu *cpu) {
    cpu->addr = cpu->pc++;
}

static void zpg(struct cpu *cpu) {
    uint8_t addr = read_byte(cpu, cpu->pc++);
    cpu->addr = addr;
}

static void zpx(struct cpu *cpu) {
    uint8_t addr = read_byte(cpu, cpu->pc++);
    read_byte(cpu, addr);
    addr += cpu->x;
    cpu->addr = addr;
}

static void zpy(struct cpu *cpu) {
    uint8_t addr = read_byte(cpu, cpu->pc++);
    read_byte(cpu, addr);
    addr += cpu->y;
    cpu->addr = addr;
}

//-----------------------------------------------------------------------------
// Instructions
//-----------------------------------------------------------------------------

static void adc(struct cpu *cpu) {
    uint8_t data = read_byte(cpu, cpu->addr);
    add(cpu, data);
}

static void alr(struct cpu *cpu) {
    cpu->a &= read_byte(cpu, cpu->addr);
    bool carry = cpu->a & 0x01;
    cpu->a >>= 1;
    cpu->p.c = carry;
    SET_ZN(cpu->a);
}

static void anc(struct cpu *cpu) {
    cpu->a &= read_byte(cpu, cpu->addr);
    cpu->p.c = cpu->a & 0x80;
    SET_ZN(cpu->a);
}

static void _and(struct cpu *cpu) {
    cpu->a &= read_byte(cpu, cpu->addr);
    SET_ZN(cpu->a);
}

static void arr(struct cpu *cpu) {
    cpu->a &= read_byte(cpu, cpu->addr);
    cpu->a >>= 1;
    cpu->a = (cpu->a & ~(1 << 7)) | cpu->p.c << 7;
    cpu->p.c = cpu->a & 0x40;
    cpu->p.v = cpu->p.c ^ (cpu->a & 0x20);
    SET_ZN(cpu->a);
}

static void asl(struct cpu *cpu) {
    uint8_t data = read_byte(cpu, cpu->addr);
    write_byte(cpu, cpu->addr, data);
    bool carry = data & 0x80;
    data <<= 1;
    write_byte(cpu, cpu->addr, data);
    cpu->p.c = carry;
    SET_ZN(data);
}

static void asl_a(struct cpu *cpu) {
    read_byte(cpu, cpu->a);
    bool carry = cpu->a & 0x80;
    cpu->a <<= 1;
    cpu->p.c = carry;
    SET_ZN(cpu->a);
}

static void bcc(struct cpu *cpu) {
    branch(cpu, !cpu->p.c);
}

static void bcs(struct cpu *cpu) {
    branch(cpu, cpu->p.c);
}

static void beq(struct cpu *cpu) {
    branch(cpu, cpu->p.z);
}

static void bit(struct cpu *cpu) {
    uint8_t data = read_byte(cpu, cpu->addr);
    struct status status;
    memcpy(&status, &data, 1);
    cpu->p.z = (cpu->a & data) == 0;
    cpu->p.v = status.v;
    cpu->p.n = status.n;
}

static void bmi(struct cpu *cpu) {
    branch(cpu, cpu->p.n);
}

static void bne(struct cpu *cpu) {
    branch(cpu, !cpu->p.z);
}

static void bpl(struct cpu *cpu) {
    branch(cpu, !cpu->p.n);
}

static void brk(struct cpu *cpu) {
    // read_byte(cpu, cpu->pc++);
    // PUSH(cpu->pc >> 8);
    // PUSH(cpu->pc);
    // TODO
}

static void bvc(struct cpu *cpu) {
    branch(cpu, !cpu->p.v);
}

static void bvs(struct cpu *cpu) {
    branch(cpu, cpu->p.v);
}

static void clc(struct cpu *cpu) {
    read_byte(cpu, cpu->pc);
    cpu->p.c = false;
}

static void cld(struct cpu *cpu) {
    read_byte(cpu, cpu->pc);
    cpu->p.d = false;
}

static void cli(struct cpu *cpu) {
    read_byte(cpu, cpu->pc);
    cpu->p.i = false;
}

static void clv(struct cpu *cpu) {
    read_byte(cpu, cpu->pc);
    cpu->p.v = false;
}

static void cmp(struct cpu *cpu) {
    uint8_t data = read_byte(cpu, cpu->addr);
    compare(cpu, cpu->a, data);
}

static void cpx(struct cpu *cpu) {
    uint8_t data = read_byte(cpu, cpu->addr);
    compare(cpu, cpu->x, data);
}

static void cpy(struct cpu *cpu) {
    uint8_t data = read_byte(cpu, cpu->addr);
    compare(cpu, cpu->y, data);
}

static void dcp(struct cpu *cpu) {
    uint8_t data = read_byte(cpu, cpu->addr);
    write_byte(cpu, cpu->addr, data);
    --data;
    write_byte(cpu, cpu->addr, data);
    compare(cpu, cpu->a, data);
}

static void dec(struct cpu *cpu) {
    uint8_t data = read_byte(cpu, cpu->addr);
    write_byte(cpu, cpu->addr, data);
    --data;
    write_byte(cpu, cpu->addr, data);
    SET_ZN(data);
}

static void dex(struct cpu *cpu) {
    read_byte(cpu, cpu->pc);
    --cpu->x;
    SET_ZN(cpu->x);
}

static void dey(struct cpu *cpu) {
    read_byte(cpu, cpu->pc);
    --cpu->y;
    SET_ZN(cpu->y);
}

static void eor(struct cpu *cpu) {
    cpu->a ^= read_byte(cpu, cpu->addr);
    SET_ZN(cpu->a);
}

static void inc(struct cpu *cpu) {
    uint8_t data = read_byte(cpu, cpu->addr);
    write_byte(cpu, cpu->addr, data);
    ++data;
    write_byte(cpu, cpu->addr, data);
    SET_ZN(data);
}

static void inx(struct cpu *cpu) {
    read_byte(cpu, cpu->pc);
    ++cpu->x;
    SET_ZN(cpu->x);
}

static void iny(struct cpu *cpu) {
    read_byte(cpu, cpu->pc);
    ++cpu->y;
    SET_ZN(cpu->y);
}

static void isc(struct cpu *cpu) {
    uint8_t data = read_byte(cpu, cpu->addr);
    write_byte(cpu, cpu->addr, data);
    ++data;
    write_byte(cpu, cpu->addr, data);
    add(cpu, data ^ 0xFF);
}

static void jmp(struct cpu *cpu) {
    cpu->pc = cpu->addr;
}

static void jsr(struct cpu *cpu) {
    uint8_t pcl = read_byte(cpu, cpu->pc++);
    PEEK();
    PUSH(cpu->pc >> 8);
    PUSH(cpu->pc);
    uint8_t pch = read_byte(cpu, cpu->pc++);
    cpu->pc = pcl | pch << 8;
}

static void las(struct cpu *cpu) {
    cpu->a = read_byte(cpu, cpu->addr) & cpu->s;
    cpu->x = cpu->a;
    cpu->s = cpu->a;
    SET_ZN(cpu->x);
}

static void lax(struct cpu *cpu) {
    cpu->a = read_byte(cpu, cpu->addr);
    cpu->x = cpu->a;
    SET_ZN(cpu->x);
}

static void lda(struct cpu *cpu) {
    cpu->a = read_byte(cpu, cpu->addr);
    SET_ZN(cpu->a);
}

static void ldx(struct cpu *cpu) {
    cpu->x = read_byte(cpu, cpu->addr);
    SET_ZN(cpu->a);
}

static void ldy(struct cpu *cpu) {
    cpu->y = read_byte(cpu, cpu->addr);
    SET_ZN(cpu->a);
}

static void ora(struct cpu *cpu) {
    cpu->a |= read_byte(cpu, cpu->addr);
    SET_ZN(cpu->a);
}

static void slo(struct cpu *cpu) {
    uint8_t data = read_byte(cpu, cpu->addr);
    write_byte(cpu, cpu->addr, data);
    bool carry = data & 0x80;
    data <<= 1;
    write_byte(cpu, cpu->addr, data);
    cpu->a |= data;
    cpu->p.c = carry;
    SET_ZN(cpu->a);
}

// TODO: Use flatten with musttail for instruction handlers later, e.g.,
// [[gnu::flatten]] static void lda_zpg(struct cpu *cpu) {
//     zpg(cpu);
//     lda(cpu);
//     [[clang::musttail]] return slo(cpu);
// }

//-----------------------------------------------------------------------------
// "Methods"
//-----------------------------------------------------------------------------

struct cpu *cpu_new() {
    struct cpu *cpu = calloc(1, sizeof(struct cpu));
    if (cpu == NULL) {
        printf("calloc failed");
        exit(EXIT_FAILURE);
    }

    return cpu;
}

uint16_t cpu_pc(const struct cpu *cpu) {
    return cpu->pc;
}

void cpu_set_pc(struct cpu *cpu, uint16_t pc) {
    cpu->pc = pc;
}

uint8_t cpu_a(const struct cpu *cpu) {
    return cpu->a;
}

void cpu_set_a(struct cpu *cpu, uint8_t a) {
    cpu->a = a;
}

uint8_t cpu_x(const struct cpu *cpu) {
    return cpu->x;
}

void cpu_set_x(struct cpu *cpu, uint8_t x) {
    cpu->x = x;
}

uint8_t cpu_y(const struct cpu *cpu) {
    return cpu->y;
}

void cpu_set_y(struct cpu *cpu, uint8_t y) {
    cpu->y = y;
}

uint8_t cpu_s(const struct cpu *cpu) {
    return cpu->s;
}

void cpu_set_s(struct cpu *cpu, uint8_t s) {
    cpu->s = s;
}

uint8_t cpu_p(const struct cpu *cpu) {
    uint8_t p = 0;
    memcpy(&p, &cpu->p, 1);
    return p;
}

void cpu_set_p(struct cpu *cpu, uint8_t p) {
    memcpy(&cpu->p, &p, 1);
}

uint8_t cpu_read(const struct cpu *cpu, uint16_t addr) {
    return cpu->ram[addr];
}

void cpu_write(struct cpu *cpu, uint16_t addr, uint8_t data) {
    cpu->ram[addr] = data;
}

void cpu_step(struct cpu *cpu) {
    const uint8_t opc = read_byte(cpu, cpu->pc++);
    // clang-format off
    switch (opc) {
    // case 0x00:        brk(cpu); break;
    case 0x01: idx(cpu); ora(cpu); break;
    case 0x03: idx(cpu); slo(cpu); break;
    // case 0x04: zpg(cpu); nop(cpu); break;
    case 0x05: zpg(cpu); ora(cpu); break;
    case 0x06: zpg(cpu); asl(cpu); break;
    case 0x07: zpg(cpu); slo(cpu); break;
    // case 0x08:        PHP(); break;
    case 0x09: imm(cpu); ora(cpu); break;
    case 0x0A:        asl_a(cpu); break;
    case 0x0B: imm(cpu); anc(cpu); break;
    // case 0x0C: abs(cpu); nop(cpu); break;
    case 0x0D: abt(cpu); ora(cpu); break;
    case 0x0E: abt(cpu); asl(cpu); break;
    case 0x0F: abt(cpu); slo(cpu); break;
    case 0x10:        bpl(cpu); break;
    case 0x11: idy(cpu); ora(cpu); break;
    // case 0x13: slo_idy_w(cpu); break;
    // case 0x14: zpx(cpu); nop(cpu); break;
    case 0x15: zpx(cpu); ora(cpu); break;
    case 0x16: zpx(cpu); asl(cpu); break;
    case 0x17: zpx(cpu); slo(cpu); break;
    case 0x18:        clc(cpu); break;
    case 0x19: aby_r(cpu); ora(cpu); break;
    // case 0x1A: imp(cpu); nop(cpu); break;
    // case 0x1B: slo_aby_w(cpu); break;
    // case 0x1C: nop_abx_r(cpu); break;
    case 0x1D: abx_r(cpu); ora(cpu); break;
    // case 0x1E: asl_abx_w(cpu); break;
    // case 0x1F: slo_abx_w(cpu); break;
    case 0x20:        jsr(cpu); break;
    case 0x21: idx(cpu); _and(cpu); break;
    // case 0x23: idx(cpu); rla(cpu); break;
    case 0x24: zpg(cpu); bit(cpu); break;
    case 0x25: zpg(cpu); _and(cpu); break;
    // case 0x26: zpg(cpu); rol(cpu); break;
    // case 0x27: zpg(cpu); rla(cpu); break;
    // case 0x28:        PLP(); break;
    case 0x29: imm(cpu); _and(cpu); break;
    // case 0x2A:        ROL_A(); break;
    // case 0x2B: imm(cpu); anc(cpu); break;
    case 0x2C: abt(cpu); bit(cpu); break;
    case 0x2D: abt(cpu); _and(cpu); break;
    // case 0x2E: abs(cpu); rol(cpu); break;
    // case 0x2F: abs(cpu); rla(cpu); break;
    case 0x30:        bmi(cpu); break;
    // case 0x31: and_idy_r(cpu); break;
    // case 0x33: rla_idy_w(cpu); break;
    // case 0x34: zpx(cpu); nop(cpu); break;
    case 0x35: zpx(cpu); _and(cpu); break;
    // case 0x36: zpx(cpu); rol(cpu); break;
    // case 0x37: zpx(cpu); rla(cpu); break;
    // case 0x38:        SEC(); break;
    // case 0x39: and_aby_r(cpu); break;
    // case 0x3A: imp(cpu); nop(cpu); break;
    // case 0x3B: rla_aby_w(cpu); break;
    // case 0x3C: nop_abx_r(cpu); break;
    // case 0x3D: and_abx_r(cpu); break;
    // case 0x3E: rol_abx_w(cpu); break;
    // case 0x3F: rla_abx_w(cpu); break;
    // case 0x40:        RTI(); break;
    case 0x41: idx(cpu); eor(cpu); break;
    // case 0x43: idx(cpu); sre(cpu); break;
    // case 0x44: zpg(cpu); nop(cpu); break;
    case 0x45: zpg(cpu); eor(cpu); break;
    // case 0x46: zpg(cpu); lsr(cpu); break;
    // case 0x47: zpg(cpu); sre(cpu); break;
    // case 0x48:        PHA(); break;
    case 0x49: imm(cpu); eor(cpu); break;
    // case 0x4A:        LSR_A(); break;
    case 0x4B: imm(cpu); alr(cpu); break;
    case 0x4C: abt(cpu); jmp(cpu); break;
    case 0x4D: abt(cpu); eor(cpu); break;
    // case 0x4E: abs(cpu); lsr(cpu); break;
    // case 0x4F: abs(cpu); sre(cpu); break;
    case 0x50:        bvc(cpu); break;
    // case 0x51: eor_idy_r(cpu); break;
    // case 0x53: sre_idy_w(cpu); break;
    // case 0x54: zpx(cpu); nop(cpu); break;
    case 0x55: zpx(cpu); eor(cpu); break;
    // case 0x56: zpx(cpu); lsr(cpu); break;
    // case 0x57: zpx(cpu); sre(cpu); break;
    case 0x58:        cli(cpu); break;
    // case 0x59: eor_aby_r(cpu); break;
    // case 0x5A: imp(cpu); nop(cpu); break;
    // case 0x5B: sre_aby_w(cpu); break;
    // case 0x5C: nop_abx_r(cpu); break;
    // case 0x5D: eor_abx_r(cpu); break;
    // case 0x5E: lsr_abx_w(cpu); break;
    // case 0x5F: sre_abx_w(cpu); break;
    // case 0x60:        RTS(); break;
    case 0x61: idx(cpu); adc(cpu); break;
    // case 0x63: idx(cpu); rra(cpu); break;
    // case 0x64: zpg(cpu); nop(cpu); break;
    case 0x65: zpg(cpu); adc(cpu); break;
    // case 0x66: zpg(cpu); ror(cpu); break;
    // case 0x67: zpg(cpu); rra(cpu); break;
    // case 0x68:        PLA(); break;
    case 0x69: imm(cpu); adc(cpu); break;
    // case 0x6A:        ROR_A(); break;
    case 0x6B: imm(cpu); arr(cpu); break;
    // case 0x6C: ind(cpu); jmp(cpu); break;
    case 0x6D: abt(cpu); adc(cpu); break;
    // case 0x6E: abs(cpu); ror(cpu); break;
    // case 0x6F: abs(cpu); rra(cpu); break;
    case 0x70:        bvs(cpu); break;
    // case 0x71: adc_idy_r(cpu); break;
    // case 0x73: rra_idy_w(cpu); break;
    // case 0x74: zpx(cpu); nop(cpu); break;
    case 0x75: zpx(cpu); adc(cpu); break;
    // case 0x76: zpx(cpu); ror(cpu); break;
    // case 0x77: zpx(cpu); rra(cpu); break;
    // case 0x78:        SEI(); break;
    // case 0x79: adc_aby_r(cpu); break;
    // case 0x7A: imp(cpu); nop(cpu); break;
    // case 0x7B: rra_aby_w(cpu); break;
    // case 0x7C: nop_abx_r(cpu); break;
    // case 0x7D: adc_abx_r(cpu); break;
    // case 0x7E: ror_abx_w(cpu); break;
    // case 0x7F: rra_abx_w(cpu); break;
    // case 0x80: imm(cpu); nop(cpu); break;
    // case 0x81: idx(cpu); sta(cpu); break;
    // case 0x82: imm(cpu); nop(cpu); break;
    // case 0x83: idx(cpu); sax(cpu); break;
    // case 0x84: zpg(cpu); sty(cpu); break;
    // case 0x85: zpg(cpu); sta(cpu); break;
    // case 0x86: zpg(cpu); stx(cpu); break;
    // case 0x87: zpg(cpu); sax(cpu); break;
    case 0x88:        dey(cpu); break;
    // case 0x89: imm(cpu); nop(cpu); break;
    // case 0x8A:        TXA(); break;
    // case 0x8B:        ANE(); break;
    // case 0x8C: abs(cpu); sty(cpu); break;
    // case 0x8D: abs(cpu); sta(cpu); break;
    // case 0x8E: abs(cpu); stx(cpu); break;
    // case 0x8F: abs(cpu); sax(cpu); break;
    case 0x90:        bcc(cpu); break;
    // case 0x91: sta_idy_w(cpu); break;
    // case 0x93: sha_idy_w(cpu); break;
    // case 0x94: zpx(cpu); sty(cpu); break;
    // case 0x95: zpx(cpu); sta(cpu); break;
    // case 0x96: zpy(cpu); stx(cpu); break;
    // case 0x97: zpy(cpu); sax(cpu); break;
    // case 0x98:        TYA(); break;
    // case 0x99: sta_aby_w(cpu); break;
    // case 0x9A:        TXS(); break;
    // case 0x9B: tas_aby_w(cpu); break;
    // case 0x9C: shy_abx_w(cpu); break;
    // case 0x9D: sta_abx_w(cpu); break;
    // case 0x9E: shx_aby_w(cpu); break;
    // case 0x9F: sha_aby_w(cpu); break;
    case 0xA0: imm(cpu); ldy(cpu); break;
    case 0xA1: idx(cpu); lda(cpu); break;
    case 0xA2: imm(cpu); ldx(cpu); break;
    case 0xA3: idx(cpu); lax(cpu); break;
    case 0xA4: zpg(cpu); ldy(cpu); break;
    case 0xA5: zpg(cpu); lda(cpu); break;
    case 0xA6: zpg(cpu); ldx(cpu); break;
    case 0xA7: zpg(cpu); lax(cpu); break;
    // case 0xA8:        TAY(); break;
    case 0xA9: imm(cpu); lda(cpu); break;
    // case 0xAA:        TAX(); break;
    // case 0xAB: imm(cpu); lxa(cpu); break;
    case 0xAC: abt(cpu); ldy(cpu); break;
    case 0xAD: abt(cpu); lda(cpu); break;
    case 0xAE: abt(cpu); ldx(cpu); break;
    case 0xAF: abt(cpu); lax(cpu); break;
    case 0xB0:        bcs(cpu); break;
    case 0xB1: idy(cpu); lda(cpu); break;
    // case 0xB3: lax_idy_r(cpu); break;
    case 0xB4: zpx(cpu); ldy(cpu); break;
    case 0xB5: zpx(cpu); lda(cpu); break;
    case 0xB6: zpy(cpu); ldx(cpu); break;
    case 0xB7: zpy(cpu); lax(cpu); break;
    case 0xB8:        clv(cpu); break;
    case 0xB9: aby_r(cpu); lda(cpu); break;
    // case 0xBA:        TSX(); break;
    // case 0xBB: las_aby_r(cpu); break;
    // case 0xBC: ldy_abx_r(cpu); break;
    case 0xBD: abx_r(cpu); lda(cpu); break;
    // case 0xBE: ldx_aby_r(cpu); break;
    // case 0xBF: lax_aby_r(cpu); break;
    case 0xC0: imm(cpu); cpy(cpu); break;
    case 0xC1: idx(cpu); cmp(cpu); break;
    // case 0xC2: imm(cpu); nop(cpu); break;
    case 0xC3: idx(cpu); dcp(cpu); break;
    // case 0xC4: zpg(cpu); cpy(cpu); break;
    case 0xC5: zpg(cpu); cmp(cpu); break;
    case 0xC6: zpg(cpu); dec(cpu); break;
    case 0xC7: zpg(cpu); dcp(cpu); break;
    case 0xC8:        iny(cpu); break;
    case 0xC9: imm(cpu); cmp(cpu); break;
    case 0xCA:        dex(cpu); break;
    // case 0xCB: imm(cpu); sbx(cpu); break;
    case 0xCC: abt(cpu); cpy(cpu); break;
    case 0xCD: abt(cpu); cmp(cpu); break;
    case 0xCE: abt(cpu); dec(cpu); break;
    case 0xCF: abt(cpu); dcp(cpu); break;
    case 0xD0:        bne(cpu); break;
    // case 0xD1: cmp_idy_r(cpu); break;
    // case 0xD3: dcp_idy_w(cpu); break;
    // case 0xD4: zpx(cpu); nop(cpu); break;
    case 0xD5: zpx(cpu); cmp(cpu); break;
    case 0xD6: zpx(cpu); dec(cpu); break;
    case 0xD7: zpx(cpu); dcp(cpu); break;
    case 0xD8:        cld(cpu); break;
    // case 0xD9: cmp_aby_r(cpu); break;
    // case 0xDA: imp(cpu); nop(cpu); break;
    // case 0xDB: dcp_aby_w(cpu); break;
    // case 0xDC: nop_abx_r(cpu); break;
    // case 0xDD: cmp_abx_r(cpu); break;
    // case 0xDE: dec_abx_w(cpu); break;
    // case 0xDF: dcp_abx_w(cpu); break;
    case 0xE0: imm(cpu); cpx(cpu); break;
    // case 0xE1: idx(cpu); sbc(cpu); break;
    // case 0xE2: imm(cpu); nop(cpu); break;
    case 0xE3: idx(cpu); isc(cpu); break;
    case 0xE4: zpg(cpu); cpx(cpu); break;
    // case 0xE5: zpg(cpu); sbc(cpu); break;
    case 0xE6: zpg(cpu); inc(cpu); break;
    case 0xE7: zpg(cpu); isc(cpu); break;
    case 0xE8:        inx(cpu); break;
    // case 0xE9: imm(cpu); sbc(cpu); break;
    // case 0xEA: imp(cpu); nop(cpu); break;
    // case 0xEB: imm(cpu); sbc(cpu); break;
    case 0xEC: abt(cpu); cpx(cpu); break;
    // case 0xED: abs(cpu); sbc(cpu); break;
    case 0xEE: abt(cpu); inc(cpu); break;
    case 0xEF: abt(cpu); isc(cpu); break;
    case 0xF0:        beq(cpu); break;
    // case 0xF1: sbc_idy_r(cpu); break;
    // case 0xF3: isc_idy_w(cpu); break;
    // case 0xF4: zpx(cpu); nop(cpu); break;
    // case 0xF5: zpx(cpu); sbc(cpu); break;
    case 0xF6: zpx(cpu); inc(cpu); break;
    case 0xF7: zpx(cpu); isc(cpu); break;
    // case 0xF8:        SED(); break;
    // case 0xF9: sbc_aby_r(cpu); break;
    // case 0xFA: imp(cpu); nop(cpu); break;
    // case 0xFB: isc_aby_w(cpu); break;
    // case 0xFC: nop_abx_r(cpu); break;
    // case 0xFD: sbc_abx_r(cpu); break;
    // case 0xFE: inc_abx_w(cpu); break;
    // case 0xFF: isc_abx_w(cpu); break;
    default:
         printf("unknown opcode: 0x%X\n", opc);
        //  exit(EXIT_FAILURE);
    }
    // clang-format on
}

void cpu_free(struct cpu *cpu) {
    free(cpu);
}

#ifdef PROCESSOR_TESTS
uint16_t cpu_addr_bus(struct cpu *cpu, size_t cycle) {
    return cpu->bus_states[cycle].addr;
}

uint8_t cpu_data_bus(struct cpu *cpu, size_t cycle) {
    return cpu->bus_states[cycle].data;
}

uint8_t cpu_cycles(struct cpu *cpu) {
    return cpu->cycles;
}

void cpu_reset_cycles(struct cpu *cpu) {
    cpu->cycles = 0;
}
#endif
