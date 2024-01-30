#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cpu.h"

#define READ(addr) cpu->ram[(addr)]

#define SET_ZN()                                                               \
    do {                                                                       \
        cpu->p.z = cpu->a == 0;                                                \
        cpu->p.n = (cpu->a & 0x80) != 0;                                       \
    } while (0)

#define ABS()                                                                  \
    do {                                                                       \
        uint8_t low = READ(cpu->pc++);                                         \
        uint8_t high = READ(cpu->pc++);                                        \
        operand = READ(low | high << 8);                                       \
    } while (0)

#define IMM()                                                                  \
    do {                                                                       \
        operand = READ(cpu->pc++);                                             \
    } while (0)

#define ZPG()                                                                  \
    do {                                                                       \
        uint8_t addr = READ(cpu->pc++);                                        \
        operand = READ(addr);                                                  \
    } while (0)

#define ZPX()                                                                  \
    do {                                                                       \
        uint8_t addr = READ(cpu->pc++) + cpu->x;                               \
        operand = READ(addr);                                                  \
    } while (0)

#define LDA()                                                                  \
    do {                                                                       \
        cpu->a = operand;                                                      \
        SET_ZN();                                                              \
    } while (0)

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
};

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

// NOLINTNEXTLINE(readability-function-cognitive-complexity)
void cpu_step(struct cpu *cpu) {
    uint8_t operand;

    uint8_t opc = cpu->ram[cpu->pc++];
    // clang-format off
    switch (opc) {
    // case 0xA5: ZPG(); LDA(); break;
    // case 0xA9: IMM(); LDA(); break;
    // case 0xAD: ABS(); LDA(); break;
    // case 0xB5: ZPX(); LDA(); break;
    // case 0x00:        BRK(); break;
    // case 0x01: IDX(); ORA(); break;
    // case 0x03: IDX(); SLO(); break;
    // case 0x04: ZPG(); NOP(); break;
    // case 0x05: ZPG(); ORA(); break;
    // case 0x06: ZPG(); ASL(); break;
    // case 0x07: ZPG(); SLO(); break;
    // case 0x08:        PHP(); break;
    // case 0x09: IMM(); ORA(); break;
    // case 0x0A:        ASL_A(); break;
    // case 0x0B: IMM(); ANC(); break;
    // case 0x0C: ABS(); NOP(); break;
    // case 0x0D: ABS(); ORA(); break;
    // case 0x0E: ABS(); ASL(); break;
    // case 0x0F: ABS(); SLO(); break;
    // case 0x10:        BPL(); break;
    // case 0x11: IDY_R(); ORA(); break;
    // case 0x13: IDY_W(); SLO(); break;
    // case 0x14: ZPX(); NOP(); break;
    // case 0x15: ZPX(); ORA(); break;
    // case 0x16: ZPX(); ASL(); break;
    // case 0x17: ZPX(); SLO(); break;
    // case 0x18:        CLC(); break;
    // case 0x19: ABY_R(); ORA(); break;
    // case 0x1A: IMP(); NOP(); break;
    // case 0x1B: ABY_W(); SLO(); break;
    // case 0x1C: ABX_R(); NOP(); break;
    // case 0x1D: ABX_R(); ORA(); break;
    // case 0x1E: ABX_W(); ASL(); break;
    // case 0x1F: ABX_W(); SLO(); break;
    // case 0x20:        JSR(); break;
    // case 0x21: IDX(); AND(); break;
    // case 0x23: IDX(); RLA(); break;
    // case 0x24: ZPG(); BIT(); break;
    // case 0x25: ZPG(); AND(); break;
    // case 0x26: ZPG(); ROL(); break;
    // case 0x27: ZPG(); RLA(); break;
    // case 0x28:        PLP(); break;
    // case 0x29: IMM(); AND(); break;
    // case 0x2A:        ROL_A(); break;
    // case 0x2B: IMM(); ANC(); break;
    // case 0x2C: ABS(); BIT(); break;
    // case 0x2D: ABS(); AND(); break;
    // case 0x2E: ABS(); ROL(); break;
    // case 0x2F: ABS(); RLA(); break;
    // case 0x30:        BMI(); break;
    // case 0x31: IDY_R(); AND(); break;
    // case 0x33: IDY_W(); RLA(); break;
    // case 0x34: ZPX(); NOP(); break;
    // case 0x35: ZPX(); AND(); break;
    // case 0x36: ZPX(); ROL(); break;
    // case 0x37: ZPX(); RLA(); break;
    // case 0x38:        SEC(); break;
    // case 0x39: ABY_R(); AND(); break;
    // case 0x3A: IMP(); NOP(); break;
    // case 0x3B: ABY_W(); RLA(); break;
    // case 0x3C: ABX_R(); NOP(); break;
    // case 0x3D: ABX_R(); AND(); break;
    // case 0x3E: ABX_W(); ROL(); break;
    // case 0x3F: ABX_W(); RLA(); break;
    // case 0x40:        RTI(); break;
    // case 0x41: IDX(); EOR(); break;
    // case 0x43: IDX(); SRE(); break;
    // case 0x44: ZPG(); NOP(); break;
    // case 0x45: ZPG(); EOR(); break;
    // case 0x46: ZPG(); LSR(); break;
    // case 0x47: ZPG(); SRE(); break;
    // case 0x48:        PHA(); break;
    // case 0x49: IMM(); EOR(); break;
    // case 0x4A:        LSR_A(); break;
    // case 0x4B: IMM(); ALR(); break;
    // case 0x4C: ABS(); JMP(); break;
    // case 0x4D: ABS(); EOR(); break;
    // case 0x4E: ABS(); LSR(); break;
    // case 0x4F: ABS(); SRE(); break;
    // case 0x50:        BVC(); break;
    // case 0x51: IDY_R(); EOR(); break;
    // case 0x53: IDY_W(); SRE(); break;
    // case 0x54: ZPX(); NOP(); break;
    // case 0x55: ZPX(); EOR(); break;
    // case 0x56: ZPX(); LSR(); break;
    // case 0x57: ZPX(); SRE(); break;
    // case 0x58:        CLI(); break;
    // case 0x59: ABY_R(); EOR(); break;
    // case 0x5A: IMP(); NOP(); break;
    // case 0x5B: ABY_W(); SRE(); break;
    // case 0x5C: ABX_R(); NOP(); break;
    // case 0x5D: ABX_R(); EOR(); break;
    // case 0x5E: ABX_W(); LSR(); break;
    // case 0x5F: ABX_W(); SRE(); break;
    // case 0x60:        RTS(); break;
    // case 0x61: IDX(); ADC(); break;
    // case 0x63: IDX(); RRA(); break;
    // case 0x64: ZPG(); NOP(); break;
    // case 0x65: ZPG(); ADC(); break;
    // case 0x66: ZPG(); ROR(); break;
    // case 0x67: ZPG(); RRA(); break;
    // case 0x68:        PLA(); break;
    // case 0x69: IMM(); ADC(); break;
    // case 0x6A:        ROR_A(); break;
    // case 0x6B: IMM(); ARR(); break;
    // case 0x6C: IND(); JMP(); break;
    // case 0x6D: ABS(); ADC(); break;
    // case 0x6E: ABS(); ROR(); break;
    // case 0x6F: ABS(); RRA(); break;
    // case 0x70:        BVS(); break;
    // case 0x71: IDY_R(); ADC(); break;
    // case 0x73: IDY_W(); RRA(); break;
    // case 0x74: ZPX(); NOP(); break;
    // case 0x75: ZPX(); ADC(); break;
    // case 0x76: ZPX(); ROR(); break;
    // case 0x77: ZPX(); RRA(); break;
    // case 0x78:        SEI(); break;
    // case 0x79: ABY_R(); ADC(); break;
    // case 0x7A: IMP(); NOP(); break;
    // case 0x7B: ABY_W(); RRA(); break;
    // case 0x7C: ABX_R(); NOP(); break;
    // case 0x7D: ABX_R(); ADC(); break;
    // case 0x7E: ABX_W(); ROR(); break;
    // case 0x7F: ABX_W(); RRA(); break;
    // case 0x80: IMM(); NOP(); break;
    // case 0x81: IDX(); STA(); break;
    // case 0x82: IMM(); NOP(); break;
    // case 0x83: IDX(); SAX(); break;
    // case 0x84: ZPG(); STY(); break;
    // case 0x85: ZPG(); STA(); break;
    // case 0x86: ZPG(); STX(); break;
    // case 0x87: ZPG(); SAX(); break;
    // case 0x88:        DEY(); break;
    // case 0x89: IMM(); NOP(); break;
    // case 0x8A:        TXA(); break;
    // case 0x8B:        ANE(); break;
    // case 0x8C: ABS(); STY(); break;
    // case 0x8D: ABS(); STA(); break;
    // case 0x8E: ABS(); STX(); break;
    // case 0x8F: ABS(); SAX(); break;
    // case 0x90:        BCC(); break;
    // case 0x91: IDY_W(); STA(); break;
    // case 0x93: IDY_W(); SHA(); break;
    // case 0x94: ZPX(); STY(); break;
    // case 0x95: ZPX(); STA(); break;
    // case 0x96: ZPY(); STX(); break;
    // case 0x97: ZPY(); SAX(); break;
    // case 0x98:        TYA(); break;
    // case 0x99: ABY_W(); STA(); break;
    // case 0x9A:        TXS(); break;
    // case 0x9B: ABY_W(); TAS(); break;
    // case 0x9C: ABX_W(); SHY(); break;
    // case 0x9D: ABX_W(); STA(); break;
    // case 0x9E: ABY_W(); SHX(); break;
    // case 0x9F: ABY_W(); SHA(); break;
    // case 0xA0: IMM(); LDY(); break;
    // case 0xA1: IDX(); LDA(); break;
    // case 0xA2: IMM(); LDX(); break;
    // case 0xA3: IDX(); LAX(); break;
    // case 0xA4: ZPG(); LDY(); break;
    case 0xA5: ZPG(); LDA(); break;
    // case 0xA6: ZPG(); LDX(); break;
    // case 0xA7: ZPG(); LAX(); break;
    // case 0xA8:        TAY(); break;
    case 0xA9: IMM(); LDA(); break;
    // case 0xAA:        TAX(); break;
    // case 0xAB: IMM(); LXA(); break;
    // case 0xAC: ABS(); LDY(); break;
    case 0xAD: ABS(); LDA(); break;
    // case 0xAE: ABS(); LDX(); break;
    // case 0xAF: ABS(); LAX(); break;
    // case 0xB0:        BCS(); break;
    // case 0xB1: IDY_R(); LDA(); break;
    // case 0xB3: IDY_R(); LAX(); break;
    // case 0xB4: ZPX(); LDY(); break;
    case 0xB5: ZPX(); LDA(); break;
    // case 0xB6: ZPY(); LDX(); break;
    // case 0xB7: ZPY(); LAX(); break;
    // case 0xB8:        CLV(); break;
    // case 0xB9: ABY_R(); LDA(); break;
    // case 0xBA:        TSX(); break;
    // case 0xBB: ABY_R(); LAS(); break;
    // case 0xBC: ABX_R(); LDY(); break;
    // case 0xBD: ABX_R(); LDA(); break;
    // case 0xBE: ABY_R(); LDX(); break;
    // case 0xBF: ABY_R(); LAX(); break;
    // case 0xC0: IMM(); CPY(); break;
    // case 0xC1: IDX(); CMP(); break;
    // case 0xC2: IMM(); NOP(); break;
    // case 0xC3: IDX(); DCP(); break;
    // case 0xC4: ZPG(); CPY(); break;
    // case 0xC5: ZPG(); CMP(); break;
    // case 0xC6: ZPG(); DEC(); break;
    // case 0xC7: ZPG(); DCP(); break;
    // case 0xC8:        INY(); break;
    // case 0xC9: IMM(); CMP(); break;
    // case 0xCA:        DEX(); break;
    // case 0xCB: IMM(); SBX(); break;
    // case 0xCC: ABS(); CPY(); break;
    // case 0xCD: ABS(); CMP(); break;
    // case 0xCE: ABS(); DEC(); break;
    // case 0xCF: ABS(); DCP(); break;
    // case 0xD0:        BNE(); break;
    // case 0xD1: IDY_R(); CMP(); break;
    // case 0xD3: IDY_W(); DCP(); break;
    // case 0xD4: ZPX(); NOP(); break;
    // case 0xD5: ZPX(); CMP(); break;
    // case 0xD6: ZPX(); DEC(); break;
    // case 0xD7: ZPX(); DCP(); break;
    // case 0xD8:        CLD(); break;
    // case 0xD9: ABY_R(); CMP(); break;
    // case 0xDA: IMP(); NOP(); break;
    // case 0xDB: ABY_W(); DCP(); break;
    // case 0xDC: ABX_R(); NOP(); break;
    // case 0xDD: ABX_R(); CMP(); break;
    // case 0xDE: ABX_W(); DEC(); break;
    // case 0xDF: ABX_W(); DCP(); break;
    // case 0xE0: IMM(); CPX(); break;
    // case 0xE1: IDX(); SBC(); break;
    // case 0xE2: IMM(); NOP(); break;
    // case 0xE3: IDX(); ISC(); break;
    // case 0xE4: ZPG(); CPX(); break;
    // case 0xE5: ZPG(); SBC(); break;
    // case 0xE6: ZPG(); INC(); break;
    // case 0xE7: ZPG(); ISC(); break;
    // case 0xE8:        INX(); break;
    // case 0xE9: IMM(); SBC(); break;
    // case 0xEA: IMP(); NOP(); break;
    // case 0xEB: IMM(); SBC(); break;
    // case 0xEC: ABS(); CPX(); break;
    // case 0xED: ABS(); SBC(); break;
    // case 0xEE: ABS(); INC(); break;
    // case 0xEF: ABS(); ISC(); break;
    // case 0xF0:        BEQ(); break;
    // case 0xF1: IDY_R(); SBC(); break;
    // case 0xF3: IDY_W(); ISC(); break;
    // case 0xF4: ZPX(); NOP(); break;
    // case 0xF5: ZPX(); SBC(); break;
    // case 0xF6: ZPX(); INC(); break;
    // case 0xF7: ZPX(); ISC(); break;
    // case 0xF8:        SED(); break;
    // case 0xF9: ABY_R(); SBC(); break;
    // case 0xFA: IMP(); NOP(); break;
    // case 0xFB: ABY_W(); ISC(); break;
    // case 0xFC: ABX_R(); NOP(); break;
    // case 0xFD: ABX_R(); SBC(); break;
    // case 0xFE: ABX_W(); INC(); break;
    // case 0xFF: ABX_W(); ISC(); break;
    default:
         printf("unknown opcode: 0x%X\n", opc);
         exit(EXIT_FAILURE);
    }
    // clang-format on
}

void cpu_free(struct cpu *cpu) {
    free(cpu);
}
