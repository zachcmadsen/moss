#include <stdio.h>

#include "cpu.h"

int main() {
    struct cpu *cpu = cpu_new();
    cpu_write(cpu, 0x0000, 0xA9);
    cpu_write(cpu, 0x0001, 0xEC);

    cpu_step(cpu);

    if (cpu_a(cpu) != 0xEC) {
        printf("failed\n");
    }

    printf("passed\n");
    return 0;
}
