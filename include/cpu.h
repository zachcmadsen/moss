#pragma once

#include <stdint.h>

struct cpu;

struct cpu *cpu_new(void);
uint16_t cpu_pc(struct cpu *cpu);
void cpu_set_pc(struct cpu *cpu, uint16_t pc);
uint8_t cpu_a(struct cpu *cpu);
void cpu_set_a(struct cpu *cpu, uint8_t a);
uint8_t cpu_x(struct cpu *cpu);
void cpu_set_x(struct cpu *cpu, uint8_t x);
uint8_t cpu_y(struct cpu *cpu);
void cpu_set_y(struct cpu *cpu, uint8_t y);
uint8_t cpu_s(struct cpu *cpu);
void cpu_set_s(struct cpu *cpu, uint8_t s);
uint8_t cpu_p(struct cpu *cpu);
void cpu_set_p(struct cpu *cpu, uint8_t p);
uint8_t cpu_read(struct cpu *cpu, uint16_t addr);
void cpu_write(struct cpu *cpu, uint16_t addr, uint8_t data);
void cpu_step(struct cpu *cpu);
void cpu_free(struct cpu *cpu);
