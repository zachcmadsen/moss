#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cpu.h"

enum { ADDR_SPACE_SIZE = 65536 };

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

    uint8_t ram[ADDR_SPACE_SIZE];

#ifdef PROCESSOR_TESTS
    struct {
        uint16_t addr;
        uint8_t data;
    } bus_states[7];
    size_t cycles;
#endif
};

#define SET_ZN(reg)                                                            \
    do {                                                                       \
        cpu->p.z = (reg) == 0;                                                 \
        cpu->p.n = ((reg) & 0x80) != 0;                                        \
    } while (0)

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

static uint8_t abx_r(struct cpu *cpu) {
    uint8_t low = read_byte(cpu, cpu->pc++) + cpu->x;
    bool overflow = low < cpu->x;
    uint8_t high = read_byte(cpu, cpu->pc++);
    if (overflow) {
        read_byte(cpu, low | high << 8);
    }
    return read_byte(cpu, low | (high + overflow) << 8);
}

static uint8_t aby_r(struct cpu *cpu) {
    uint8_t low = read_byte(cpu, cpu->pc++) + cpu->y;
    bool overflow = low < cpu->y;
    uint8_t high = read_byte(cpu, cpu->pc++);
    if (overflow) {
        read_byte(cpu, low | high << 8);
    }
    return read_byte(cpu, low | (high + overflow) << 8);
}

static uint8_t abl(struct cpu *cpu) {
    uint8_t low = read_byte(cpu, cpu->pc++);
    uint8_t high = read_byte(cpu, cpu->pc++);
    uint16_t addr = low | high << 8;
    return read_byte(cpu, addr);
}

static uint8_t idx(struct cpu *cpu) {
    uint8_t ptr = read_byte(cpu, cpu->pc++);
    read_byte(cpu, ptr);
    ptr += cpu->x;
    uint8_t low = read_byte(cpu, ptr);
    uint8_t high = read_byte(cpu, (uint8_t)(ptr + 1));
    uint16_t addr = low | high << 8;
    return read_byte(cpu, addr);
}

static uint8_t idy(struct cpu *cpu) {
    uint8_t ptr = read_byte(cpu, cpu->pc++);
    uint8_t low = read_byte(cpu, ptr) + cpu->y;
    bool overflow = low < cpu->y;
    uint8_t high = read_byte(cpu, (uint8_t)(ptr + 1));
    if (overflow) {
        read_byte(cpu, low | high << 8);
    }

    return read_byte(cpu, low | (high + overflow) << 8);
}

static uint8_t zpg(struct cpu *cpu) {
    uint8_t addr = read_byte(cpu, cpu->pc++);
    return read_byte(cpu, addr);
}

static uint8_t zpx(struct cpu *cpu) {
    uint8_t addr = read_byte(cpu, cpu->pc++);
    read_byte(cpu, addr);
    addr += cpu->x;
    return read_byte(cpu, addr);
}

static uint8_t zpy(struct cpu *cpu) {
    uint8_t addr = read_byte(cpu, cpu->pc++);
    read_byte(cpu, addr);
    addr += cpu->x;
    return read_byte(cpu, addr);
}

static void lda_abs(struct cpu *cpu) {
    cpu->a = abl(cpu);
    SET_ZN(cpu->a);
}

static void lda_abx(struct cpu *cpu) {
    cpu->a = abx_r(cpu);
    SET_ZN(cpu->a);
}

static void lda_aby(struct cpu *cpu) {
    cpu->a = aby_r(cpu);
    SET_ZN(cpu->a);
}

static void lda_idx(struct cpu *cpu) {
    cpu->a = idx(cpu);
    SET_ZN(cpu->a);
}

static void lda_idy(struct cpu *cpu) {
    cpu->a = idy(cpu);
    SET_ZN(cpu->a);
}

static void lda_imm(struct cpu *cpu) {
    cpu->a = read_byte(cpu, cpu->pc++);
    SET_ZN(cpu->a);
}

static void lda_zpg(struct cpu *cpu) {
    cpu->a = zpg(cpu);
    SET_ZN(cpu->a);
}

static void lda_zpx(struct cpu *cpu) {
    cpu->a = zpx(cpu);
    SET_ZN(cpu->a);
}

static void ora_abs(struct cpu *cpu) {
    cpu->a |= abl(cpu);
    SET_ZN(cpu->a);
}

static void ora_abx(struct cpu *cpu) {
    cpu->a |= abx_r(cpu);
    SET_ZN(cpu->a);
}

static void ora_aby(struct cpu *cpu) {
    cpu->a |= aby_r(cpu);
    SET_ZN(cpu->a);
}

static void ora_idx(struct cpu *cpu) {
    cpu->a |= idx(cpu);
    SET_ZN(cpu->a);
}

static void ora_idy(struct cpu *cpu) {
    cpu->a |= idy(cpu);
    SET_ZN(cpu->a);
}

static void ora_imm(struct cpu *cpu) {
    cpu->a |= read_byte(cpu, cpu->pc++);
    SET_ZN(cpu->a);
}

static void ora_zpg(struct cpu *cpu) {
    cpu->a |= zpg(cpu);
    SET_ZN(cpu->a);
}

static void ora_zpx(struct cpu *cpu) {
    cpu->a |= zpx(cpu);
    SET_ZN(cpu->a);
}

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
    // case 0x00:        BRK(); break;
    case 0x01: ora_idx(cpu); break;
    // case 0x03: slo_idx(cpu); break;
    // case 0x04: nop_zpg(cpu); break;
    case 0x05: ora_zpg(cpu); break;
    // case 0x06: asl_zpg(cpu); break;
    // case 0x07: slo_zpg(cpu); break;
    // case 0x08:        PHP(); break;
    case 0x09: ora_imm(cpu); break;
    // case 0x0A:        ASL_A(); break;
    // case 0x0B: anc_imm(cpu); break;
    // case 0x0C: nop_abs(cpu); break;
    case 0x0D: ora_abs(cpu); break;
    // case 0x0E: asl_abs(cpu); break;
    // case 0x0F: slo_abs(cpu); break;
    // case 0x10:        BPL(); break;
    case 0x11: ora_idy(cpu); break;
    // case 0x13: slo_idy_w(cpu); break;
    // case 0x14: nop_zpx(cpu); break;
    case 0x15: ora_zpx(cpu); break;
    // case 0x16: asl_zpx(cpu); break;
    // case 0x17: slo_zpx(cpu); break;
    // case 0x18:        CLC(); break;
    case 0x19: ora_aby(cpu); break;
    // case 0x1A: nop_imp(cpu); break;
    // case 0x1B: slo_aby_w(cpu); break;
    // case 0x1C: nop_abx_r(cpu); break;
    case 0x1D: ora_abx(cpu); break;
    // case 0x1E: asl_abx_w(cpu); break;
    // case 0x1F: slo_abx_w(cpu); break;
    // case 0x20:        JSR(); break;
    // case 0x21: and_idx(cpu); break;
    // case 0x23: rla_idx(cpu); break;
    // case 0x24: bit_zpg(cpu); break;
    // case 0x25: and_zpg(cpu); break;
    // case 0x26: rol_zpg(cpu); break;
    // case 0x27: rla_zpg(cpu); break;
    // case 0x28:        PLP(); break;
    // case 0x29: and_imm(cpu); break;
    // case 0x2A:        ROL_A(); break;
    // case 0x2B: anc_imm(cpu); break;
    // case 0x2C: bit_abs(cpu); break;
    // case 0x2D: and_abs(cpu); break;
    // case 0x2E: rol_abs(cpu); break;
    // case 0x2F: rla_abs(cpu); break;
    // case 0x30:        BMI(); break;
    // case 0x31: and_idy_r(cpu); break;
    // case 0x33: rla_idy_w(cpu); break;
    // case 0x34: nop_zpx(cpu); break;
    // case 0x35: and_zpx(cpu); break;
    // case 0x36: rol_zpx(cpu); break;
    // case 0x37: rla_zpx(cpu); break;
    // case 0x38:        SEC(); break;
    // case 0x39: and_aby_r(cpu); break;
    // case 0x3A: nop_imp(cpu); break;
    // case 0x3B: rla_aby_w(cpu); break;
    // case 0x3C: nop_abx_r(cpu); break;
    // case 0x3D: and_abx_r(cpu); break;
    // case 0x3E: rol_abx_w(cpu); break;
    // case 0x3F: rla_abx_w(cpu); break;
    // case 0x40:        RTI(); break;
    // case 0x41: eor_idx(cpu); break;
    // case 0x43: sre_idx(cpu); break;
    // case 0x44: nop_zpg(cpu); break;
    // case 0x45: eor_zpg(cpu); break;
    // case 0x46: lsr_zpg(cpu); break;
    // case 0x47: sre_zpg(cpu); break;
    // case 0x48:        PHA(); break;
    // case 0x49: eor_imm(cpu); break;
    // case 0x4A:        LSR_A(); break;
    // case 0x4B: alr_imm(cpu); break;
    // case 0x4C: jmp_abs(cpu); break;
    // case 0x4D: eor_abs(cpu); break;
    // case 0x4E: lsr_abs(cpu); break;
    // case 0x4F: sre_abs(cpu); break;
    // case 0x50:        BVC(); break;
    // case 0x51: eor_idy_r(cpu); break;
    // case 0x53: sre_idy_w(cpu); break;
    // case 0x54: nop_zpx(cpu); break;
    // case 0x55: eor_zpx(cpu); break;
    // case 0x56: lsr_zpx(cpu); break;
    // case 0x57: sre_zpx(cpu); break;
    // case 0x58:        CLI(); break;
    // case 0x59: eor_aby_r(cpu); break;
    // case 0x5A: nop_imp(cpu); break;
    // case 0x5B: sre_aby_w(cpu); break;
    // case 0x5C: nop_abx_r(cpu); break;
    // case 0x5D: eor_abx_r(cpu); break;
    // case 0x5E: lsr_abx_w(cpu); break;
    // case 0x5F: sre_abx_w(cpu); break;
    // case 0x60:        RTS(); break;
    // case 0x61: adc_idx(cpu); break;
    // case 0x63: rra_idx(cpu); break;
    // case 0x64: nop_zpg(cpu); break;
    // case 0x65: adc_zpg(cpu); break;
    // case 0x66: ror_zpg(cpu); break;
    // case 0x67: rra_zpg(cpu); break;
    // case 0x68:        PLA(); break;
    // case 0x69: adc_imm(cpu); break;
    // case 0x6A:        ROR_A(); break;
    // case 0x6B: arr_imm(cpu); break;
    // case 0x6C: jmp_ind(cpu); break;
    // case 0x6D: adc_abs(cpu); break;
    // case 0x6E: ror_abs(cpu); break;
    // case 0x6F: rra_abs(cpu); break;
    // case 0x70:        BVS(); break;
    // case 0x71: adc_idy_r(cpu); break;
    // case 0x73: rra_idy_w(cpu); break;
    // case 0x74: nop_zpx(cpu); break;
    // case 0x75: adc_zpx(cpu); break;
    // case 0x76: ror_zpx(cpu); break;
    // case 0x77: rra_zpx(cpu); break;
    // case 0x78:        SEI(); break;
    // case 0x79: adc_aby_r(cpu); break;
    // case 0x7A: nop_imp(cpu); break;
    // case 0x7B: rra_aby_w(cpu); break;
    // case 0x7C: nop_abx_r(cpu); break;
    // case 0x7D: adc_abx_r(cpu); break;
    // case 0x7E: ror_abx_w(cpu); break;
    // case 0x7F: rra_abx_w(cpu); break;
    // case 0x80: nop_imm(cpu); break;
    // case 0x81: sta_idx(cpu); break;
    // case 0x82: nop_imm(cpu); break;
    // case 0x83: sax_idx(cpu); break;
    // case 0x84: sty_zpg(cpu); break;
    // case 0x85: sta_zpg(cpu); break;
    // case 0x86: stx_zpg(cpu); break;
    // case 0x87: sax_zpg(cpu); break;
    // case 0x88:        DEY(); break;
    // case 0x89: nop_imm(cpu); break;
    // case 0x8A:        TXA(); break;
    // case 0x8B:        ANE(); break;
    // case 0x8C: sty_abs(cpu); break;
    // case 0x8D: sta_abs(cpu); break;
    // case 0x8E: stx_abs(cpu); break;
    // case 0x8F: sax_abs(cpu); break;
    // case 0x90:        BCC(); break;
    // case 0x91: sta_idy_w(cpu); break;
    // case 0x93: sha_idy_w(cpu); break;
    // case 0x94: sty_zpx(cpu); break;
    // case 0x95: sta_zpx(cpu); break;
    // case 0x96: stx_zpy(cpu); break;
    // case 0x97: sax_zpy(cpu); break;
    // case 0x98:        TYA(); break;
    // case 0x99: sta_aby_w(cpu); break;
    // case 0x9A:        TXS(); break;
    // case 0x9B: tas_aby_w(cpu); break;
    // case 0x9C: shy_abx_w(cpu); break;
    // case 0x9D: sta_abx_w(cpu); break;
    // case 0x9E: shx_aby_w(cpu); break;
    // case 0x9F: sha_aby_w(cpu); break;
    // case 0xA0: ldy_imm(cpu); break;
    case 0xA1: lda_idx(cpu); break;
    // case 0xA2: ldx_imm(cpu); break;
    // case 0xA3: lax_idx(cpu); break;
    // case 0xA4: ldy_zpg(cpu); break;
    case 0xA5: lda_zpg(cpu); break;
    // case 0xA6: ldx_zpg(cpu); break;
    // case 0xA7: lax_zpg(cpu); break;
    // case 0xA8:        TAY(); break;
    case 0xA9: lda_imm(cpu); break;
    // case 0xAA:        TAX(); break;
    // case 0xAB: lxa_imm(cpu); break;
    // case 0xAC: ldy_abs(cpu); break;
    case 0xAD: lda_abs(cpu); break;
    // case 0xAE: ldx_abs(cpu); break;
    // case 0xAF: lax_abs(cpu); break;
    // case 0xB0:        BCS(); break;
    case 0xB1: lda_idy(cpu); break;
    // case 0xB3: lax_idy_r(cpu); break;
    // case 0xB4: ldy_zpx(cpu); break;
    case 0xB5: lda_zpx(cpu); break;
    // case 0xB6: ldx_zpy(cpu); break;
    // case 0xB7: lax_zpy(cpu); break;
    // case 0xB8:        CLV(); break;
    case 0xB9: lda_aby(cpu); break;
    // case 0xBA:        TSX(); break;
    // case 0xBB: las_aby_r(cpu); break;
    // case 0xBC: ldy_abx_r(cpu); break;
    case 0xBD: lda_abx(cpu); break;
    // case 0xBE: ldx_aby_r(cpu); break;
    // case 0xBF: lax_aby_r(cpu); break;
    // case 0xC0: cpy_imm(cpu); break;
    // case 0xC1: cmp_idx(cpu); break;
    // case 0xC2: nop_imm(cpu); break;
    // case 0xC3: dcp_idx(cpu); break;
    // case 0xC4: cpy_zpg(cpu); break;
    // case 0xC5: cmp_zpg(cpu); break;
    // case 0xC6: dec_zpg(cpu); break;
    // case 0xC7: dcp_zpg(cpu); break;
    // case 0xC8:        INY(); break;
    // case 0xC9: cmp_imm(cpu); break;
    // case 0xCA:        DEX(); break;
    // case 0xCB: sbx_imm(cpu); break;
    // case 0xCC: cpy_abs(cpu); break;
    // case 0xCD: cmp_abs(cpu); break;
    // case 0xCE: dec_abs(cpu); break;
    // case 0xCF: dcp_abs(cpu); break;
    // case 0xD0:        BNE(); break;
    // case 0xD1: cmp_idy_r(cpu); break;
    // case 0xD3: dcp_idy_w(cpu); break;
    // case 0xD4: nop_zpx(cpu); break;
    // case 0xD5: cmp_zpx(cpu); break;
    // case 0xD6: dec_zpx(cpu); break;
    // case 0xD7: dcp_zpx(cpu); break;
    // case 0xD8:        CLD(); break;
    // case 0xD9: cmp_aby_r(cpu); break;
    // case 0xDA: nop_imp(cpu); break;
    // case 0xDB: dcp_aby_w(cpu); break;
    // case 0xDC: nop_abx_r(cpu); break;
    // case 0xDD: cmp_abx_r(cpu); break;
    // case 0xDE: dec_abx_w(cpu); break;
    // case 0xDF: dcp_abx_w(cpu); break;
    // case 0xE0: cpx_imm(cpu); break;
    // case 0xE1: sbc_idx(cpu); break;
    // case 0xE2: nop_imm(cpu); break;
    // case 0xE3: isc_idx(cpu); break;
    // case 0xE4: cpx_zpg(cpu); break;
    // case 0xE5: sbc_zpg(cpu); break;
    // case 0xE6: inc_zpg(cpu); break;
    // case 0xE7: isc_zpg(cpu); break;
    // case 0xE8:        INX(); break;
    // case 0xE9: sbc_imm(cpu); break;
    // case 0xEA: nop_imp(cpu); break;
    // case 0xEB: sbc_imm(cpu); break;
    // case 0xEC: cpx_abs(cpu); break;
    // case 0xED: sbc_abs(cpu); break;
    // case 0xEE: inc_abs(cpu); break;
    // case 0xEF: isc_abs(cpu); break;
    // case 0xF0:        BEQ(); break;
    // case 0xF1: sbc_idy_r(cpu); break;
    // case 0xF3: isc_idy_w(cpu); break;
    // case 0xF4: nop_zpx(cpu); break;
    // case 0xF5: sbc_zpx(cpu); break;
    // case 0xF6: inc_zpx(cpu); break;
    // case 0xF7: isc_zpx(cpu); break;
    // case 0xF8:        SED(); break;
    // case 0xF9: sbc_aby_r(cpu); break;
    // case 0xFA: nop_imp(cpu); break;
    // case 0xFB: isc_aby_w(cpu); break;
    // case 0xFC: nop_abx_r(cpu); break;
    // case 0xFD: sbc_abx_r(cpu); break;
    // case 0xFE: inc_abx_w(cpu); break;
    // case 0xFF: isc_abx_w(cpu); break;
    default:
         printf("unknown opcode: 0x%X\n", opc);
         exit(EXIT_FAILURE);
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
