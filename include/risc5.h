#ifndef RISC5_H
#define RISC5_H

#include <stdint.h>
#include "cpu.h"
#define OPCODE(inst) ((inst) & 0x7F)
#define RD(inst) (((inst) >> 7) & 0x1F)
#define FUNCT3(inst) (((inst) >> 12) & 0x7)
#define RS1(inst) (((inst) >> 15) & 0x1F)
#define RS2(inst) (((inst) >> 20) & 0x1F)
#define FUNCT7(inst) (((inst) >> 25) & 0x7F)
#define OP_LUI     0x37
#define OP_AUIPC   0x17
#define OP_JAL     0x6F
#define OP_JALR    0x67
#define OP_BRANCH  0x63
#define OP_LOAD    0x03
#define OP_STORE   0x23
#define OP_IMM     0x13
#define OP_OP      0x33
#define OP_SYSTEM  0x73
#define BR_EQ   0x0
#define BR_NE   0x1
#define BR_LT   0x4
#define BR_GE   0x5
#define BR_LTU  0x6
#define BR_GEU  0x7
#define ALU_ADD  0x0
#define ALU_SUB  0x0
#define ALU_SLL  0x1
#define ALU_SLT  0x2
#define ALU_SLTU 0x3
#define ALU_XOR  0x4
#define ALU_SRL  0x5
#define ALU_SRA  0x5
#define ALU_OR   0x6
#define ALU_AND  0x7
#define ALU_FUNCT7_ADD  0x00
#define ALU_FUNCT7_SUB  0x20
#define UART_BASE_ADDR 0x10000000
#define UART_DATA_REG  (UART_BASE_ADDR + 0x0)
#define UART_STATUS_REG (UART_BASE_ADDR + 0x4)
int risc5_emulator_init(void);
void risc5_emulator_run(CPU *cpu);
void risc5_emulator_step(CPU *cpu);
#endif