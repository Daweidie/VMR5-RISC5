#ifndef CPU_H
#define CPU_H

#include <stdint.h>
#include <stdbool.h>

#define NUM_REGISTERS 32
#define MEM_SIZE 1024 * 1024

typedef struct {
    uint32_t registers[NUM_REGISTERS];
    uint32_t pc;
    uint8_t *memory;
    uint32_t mem_size;
} CPU;
CPU* cpu_init(void);
void cpu_free(CPU *cpu);
void cpu_reset(CPU *cpu);
int cpu_step(CPU *cpu);
void cpu_load_program(CPU *cpu, uint8_t *program, uint32_t size, uint32_t addr);

#endif