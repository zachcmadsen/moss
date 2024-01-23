#pragma once

#include <stdint.h>

struct cpu;

struct cpu *cpu_new(void);
uint16_t cpu_pc(struct cpu *);
void cpu_set_pc(struct cpu *, uint16_t);
uint8_t cpu_a(struct cpu *);
void cpu_set_a(struct cpu *, uint8_t);
uint8_t cpu_x(struct cpu *);
void cpu_set_x(struct cpu *, uint8_t);
uint8_t cpu_y(struct cpu *);
void cpu_set_y(struct cpu *, uint8_t);
uint8_t cpu_s(struct cpu *);
void cpu_set_s(struct cpu *, uint8_t);
uint8_t cpu_p(struct cpu *);
void cpu_set_p(struct cpu *, uint8_t);
uint8_t cpu_read(struct cpu *, uint16_t);
void cpu_write(struct cpu *, uint16_t, uint8_t);
void cpu_step(struct cpu *);
void cpu_free(struct cpu *);
