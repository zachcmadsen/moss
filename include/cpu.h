#pragma once

#include <stdint.h>

struct cpu;

struct cpu *cpu_new(void);
uint8_t cpu_a(struct cpu *);
void cpu_set_ram(struct cpu *, uint16_t, uint8_t);
void cpu_step(struct cpu *);
void cpu_free(struct cpu *);
