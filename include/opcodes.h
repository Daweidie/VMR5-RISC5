#ifndef OPCODES_H
#define OPCODES_H

#include "cpu.h"
#include <stdint.h>
typedef int (*opcode_handler)(CPU *cpu, uint32_t instruction);
int execute_instruction(CPU *cpu, uint32_t instruction);
int handle_lui(CPU *cpu, uint32_t instruction);
int handle_auipc(CPU *cpu, uint32_t instruction);
int handle_jal(CPU *cpu, uint32_t instruction);
int handle_jalr(CPU *cpu, uint32_t instruction);
int handle_branch(CPU *cpu, uint32_t instruction);
int handle_load(CPU *cpu, uint32_t instruction);
int handle_store(CPU *cpu, uint32_t instruction);
int handle_immediate(CPU *cpu, uint32_t instruction);
int handle_operation(CPU *cpu, uint32_t instruction);
int handle_system(CPU *cpu, uint32_t instruction);
uint32_t sign_extend(uint32_t x, int bit_count);
int32_t sign_extend_to_int(uint32_t x, int bit_count);

#endif