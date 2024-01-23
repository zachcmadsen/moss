#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "cpu.h"

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
    uint8_t *ram;

    uint16_t pc;
    uint8_t a;
    uint8_t x;
    uint8_t y;
    uint8_t s;
    struct status p;
};

struct cpu *cpu_new() {
    uint8_t *ram = calloc(0x10000, sizeof(uint8_t));
    if (!ram) {
        exit(EXIT_FAILURE);
    }
    struct cpu *cpu = calloc(1, sizeof(struct cpu));
    if (!cpu) {
        exit(EXIT_FAILURE);
    }
    cpu->ram = ram;
    return cpu;
}

uint8_t cpu_a(struct cpu *cpu) {
    return cpu->a;
}

void cpu_set_ram(struct cpu *cpu, uint16_t addr, uint8_t data) {
    cpu->ram[addr] = data;
}

void cpu_step(struct cpu *cpu) {
    uint8_t opc = cpu->ram[cpu->pc++];

    switch (opc) {
    case 0xA9:
        cpu->a = cpu->ram[cpu->pc++];
        cpu->p.z = cpu->a == 0;
        cpu->p.n = (cpu->a & 0x80) != 0;
        break;
    default:
        printf("unknown opcode: 0x%X\n", opc);
        exit(EXIT_FAILURE);
    }
}

void cpu_free(struct cpu *cpu) {
    free(cpu->ram);
    free(cpu);
    cpu = nullptr;
}
