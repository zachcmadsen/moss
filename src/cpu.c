#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cpu.h"

#define SET_ZN(cpu)                                                            \
    do {                                                                       \
        (cpu)->p.z = (cpu)->a == 0;                                            \
        (cpu)->p.n = ((cpu)->a & 0x80) != 0;                                   \
    } while (0)

#define ABS(cpu)                                                               \
    uint16_t addr;                                                             \
    do {                                                                       \
        uint8_t low = (cpu)->ram[(cpu)->pc++];                                 \
        uint8_t high = (cpu)->ram[(cpu)->pc++];                                \
        addr = low | high << 8;                                                \
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

static void lda_abs(struct cpu *cpu) {
    ABS(cpu);
    cpu->a = cpu->ram[addr];
    SET_ZN(cpu);
}

static void lda_imm(struct cpu *cpu) {
    cpu->a = cpu->ram[cpu->pc++];
    SET_ZN(cpu);
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
    memcpy_s(&p, sizeof(p), &cpu->p, sizeof(cpu->p));
    return p;
}

void cpu_set_p(struct cpu *cpu, uint8_t p) {
    memcpy_s(&cpu->p, sizeof(cpu->p), &p, sizeof(p));
}

uint8_t cpu_read(const struct cpu *cpu, uint16_t addr) {
    return cpu->ram[addr];
}

void cpu_write(struct cpu *cpu, uint16_t addr, uint8_t data) {
    cpu->ram[addr] = data;
}

void cpu_step(struct cpu *cpu) {
    uint8_t opc = cpu->ram[cpu->pc++];

    switch (opc) {
    case 0xA9:
        lda_imm(cpu);
        break;
    case 0xAD:
        lda_abs(cpu);
        break;
    default:
        printf("unknown opcode: 0x%X\n", opc);
        exit(EXIT_FAILURE);
    }
}

void cpu_free(struct cpu *cpu) {
    free(cpu);
}
