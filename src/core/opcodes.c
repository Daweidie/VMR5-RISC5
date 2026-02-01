#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "../include/cpu.h"
#include "../include/risc5.h"
#include "../include/opcodes.h"
#include "../include/log.h"
uint32_t sign_extend(uint32_t x, int bit_count) {//扩展符号位
    if ((x >> (bit_count - 1)) & 1) {//检查最高位是否为1
        x |= ~((1 << bit_count) - 1);//如果是1，扩展符号位
    }//否则不变
    return x;//返回扩展后的值
}

int32_t sign_extend_to_int(uint32_t x, int bit_count) {//扩展符号位
    uint32_t mask = 1U << (bit_count - 1);//获取符号位掩码
    return (x ^ mask) - mask;//根据符号位掩码进行符号扩展
}//将无符号数扩展为有符号数

int execute_instruction(CPU *cpu, uint32_t instruction) {//执行指令
    if (instruction == 0x00000013) {//NOP
        DEBUG_LOG("NOP instruction executed");//打印信息
        return 0;//返回成功
    }//NOP
    if (instruction == 0x00000000) {//处理全0指令（可能是未分配的内存或填充）
        WARN_LOG("Encountered zero instruction (possibly unallocated memory or padding) at PC 0x%08X", cpu->pc);
        return 0;//忽略并继续执行
    }
    
    uint8_t opcode = OPCODE(instruction);//获取指令opcode
    switch (opcode) {//根据opcode处理指令
        case OP_LUI:
            return handle_lui(cpu, instruction);
        case OP_AUIPC://AUIPC
            return handle_auipc(cpu, instruction);
        case OP_JAL://AL
            return handle_jal(cpu, instruction);
        case OP_JALR://JALR
            return handle_jalr(cpu, instruction);
        case OP_BRANCH://分支
            return handle_branch(cpu, instruction);
        case OP_LOAD://加载
            return handle_load(cpu, instruction);
        case OP_STORE://存储
            return handle_store(cpu, instruction);
        case OP_IMM://立即数
            return handle_immediate(cpu, instruction);
        case OP_OP://算术逻辑
            return handle_operation(cpu, instruction);
        case OP_SYSTEM://系统
            return handle_system(cpu, instruction);
        default:
            ERROR_LOG("Unknown opcode: 0x%02X at PC 0x%08X", opcode, cpu->pc);
            return -1;
    }
}

int handle_lui(CPU *cpu, uint32_t instruction) {//LUI指令处理
    uint8_t rd = RD(instruction);//获取目标寄存器
    uint32_t imm = instruction & 0xFFFFF000;//20bit
    cpu->registers[rd] = imm;//将立即数加载到寄存器
    DEBUG_LOG("LUI x%d <- 0x%08X", rd, imm);//打印信息
    return 0;//返回成功
}

int handle_auipc(CPU *cpu, uint32_t instruction) {
    uint8_t rd = RD(instruction);//获取目标寄存器
    uint32_t imm = instruction & 0xFFFFF000;//获取20位立即数
    uint32_t pc = cpu->pc;//获取当前程序计数器值
    cpu->registers[rd] = pc + imm;//将立即数加上当前PC值存入目标寄存器
    DEBUG_LOG("AUIPC x%d <- 0x%08X", rd, pc + imm);//打印信息
    return 0;
}

int handle_jal(CPU *cpu, uint32_t instruction) {//JAL指令处理
    uint8_t rd = RD(instruction);//获取目标寄存器
    int32_t imm20 = (instruction >> 31) & 1;// imm[20]
    int32_t imm10_1 = (instruction >> 21) & 0x3FF;// imm[10:1]
    int32_t imm11 = (instruction >> 20) & 1;// imm[11]
    int32_t imm19_12 = (instruction >> 12) & 0xFF;// imm[19:12]
    int32_t imm = (imm20 << 20) | (imm19_12 << 12) | (imm11 << 11) | (imm10_1 << 1);// 21bit
    imm = sign_extend_to_int(imm, 21);//扩展符号位
    uint32_t target = cpu->pc + imm;//获取跳转目标地址
    if (rd != 0) {//如果目标寄存器不为零
        cpu->registers[rd] = cpu->pc + 4;//将返回地址存入目标寄存器
    }
    
    cpu->pc = target - 4;//跳转到目标地址
    DEBUG_LOG("JAL x%d <- 0x%08X", rd, target);//打印信息
    return 0;//返回成功
}
int handle_jalr(CPU *cpu, uint32_t instruction){//JALR指令处理
    uint8_t rd = RD(instruction);//获取目标寄存器
    uint8_t rs1 = RS1(instruction);//获取源寄存器
    uint32_t imm11_0 = (instruction >> 20) & 0xFFF;
    
    int32_t imm = sign_extend_to_int(imm11_0, 12);
    uint32_t target = (cpu->registers[rs1] + imm) & ~1U;
    
    if (rd != 0) {
        cpu->registers[rd] = cpu->pc + 4;
    }
    
    cpu->pc = target - 4;
    DEBUG_LOG("JALR x%d <- x%d + %d -> 0x%08X", rd, rs1, imm, target);
    
    return 0;
}

int handle_branch(CPU *cpu, uint32_t instruction) {
    uint8_t funct3 = FUNCT3(instruction);
    uint8_t rs1 = RS1(instruction);
    uint8_t rs2 = RS2(instruction);
    int32_t imm12 = (instruction >> 31) & 1;// imm[12]
    int32_t imm10_5 = (instruction >> 25) & 0x3F;// imm[10:5]
    int32_t imm4_1 = (instruction >> 8) & 0xF;// imm[4:1]
    int32_t imm11 = (instruction >> 7) & 1;// imm[11]
    
    int32_t imm = (imm12 << 12) | (imm11 << 11) | (imm10_5 << 5) | (imm4_1 << 1);
    imm = sign_extend_to_int(imm, 13);
    
    uint32_t next_pc = cpu->pc + imm;
    bool branch_taken = false;
    
    switch (funct3) {
        case BR_EQ:
            branch_taken = (cpu->registers[rs1] == cpu->registers[rs2]);
            break;
        case BR_NE:
            branch_taken = (cpu->registers[rs1] != cpu->registers[rs2]);
            break;
        case BR_LT:
            branch_taken = ((int32_t)cpu->registers[rs1] < (int32_t)cpu->registers[rs2]);
            break;
        case BR_GE:
            branch_taken = ((int32_t)cpu->registers[rs1] >= (int32_t)cpu->registers[rs2]);
            break;
        case BR_LTU:
            branch_taken = (cpu->registers[rs1] < cpu->registers[rs2]);
            break;
        case BR_GEU:
            branch_taken = (cpu->registers[rs1] >= cpu->registers[rs2]);
            break;
        default:
            ERROR_LOG("Invalid branch funct3: 0x%X", funct3);
            return -1;
    }
    
    if (branch_taken) {
        cpu->pc = next_pc - 4;
        DEBUG_LOG("BRANCH taken to 0x%08X", next_pc);
    } else {
        DEBUG_LOG("BRANCH not taken");
    }
    
    return 0;
}/*
它解析分支指令的操作码和立即数
根据不同的分支类型
比较两个寄存器的值
决定是否跳转到目标地址
分支成功时更新程序计数器pc
否则继续顺序执行
*/

int handle_load(CPU *cpu, uint32_t instruction) {
    uint8_t rd = RD(instruction);
    uint8_t rs1 = RS1(instruction);
    uint8_t funct3 = FUNCT3(instruction);
    uint32_t imm11_0 = (instruction >> 20) & 0xFFF;
    
    int32_t offset = sign_extend_to_int(imm11_0, 12);
    uint32_t addr = cpu->registers[rs1] + offset;
    if (addr == UART_DATA_REG) {
        printf("%c", (char)(cpu->registers[(RS2(instruction))] & 0xFF));
        fflush(stdout);
        return 0;
    }
    
    if (addr >= cpu->mem_size) {
        ERROR_LOG("Load from invalid memory address: 0x%08X", addr);
        return -1;
    }
    
    uint32_t value = 0;
    switch (funct3) {
        case 0x0:
            value = (int8_t)cpu->memory[addr];
            break;
        case 0x1:
            if (addr & 1) {
                ERROR_LOG("Unaligned LH at address: 0x%08X", addr);
                return -1;
            }
            value = (int16_t)((cpu->memory[addr+1] << 8) | cpu->memory[addr]);
            value = sign_extend_to_int(value, 16);
            break;
        case 0x2:
            if (addr & 3) {
                ERROR_LOG("Unaligned LW at address: 0x%08X", addr);
                return -1;
            }
            value = (cpu->memory[addr+3] << 24) | (cpu->memory[addr+2] << 16) | 
                    (cpu->memory[addr+1] << 8) | cpu->memory[addr];
            break;
        case 0x4:
            value = cpu->memory[addr];
            break;
        case 0x5:
            if (addr & 1) {
                ERROR_LOG("Unaligned LHU at address: 0x%08X", addr);
                return -1;
            }
            value = (cpu->memory[addr+1] << 8) | cpu->memory[addr];
            break;
        default:
            ERROR_LOG("Invalid load funct3: 0x%X", funct3);
            return -1;
    }
    
    cpu->registers[rd] = value;
    DEBUG_LOG("LOAD %d <- 0x%08X", rd, value);
    
    return 0;
}

int handle_store(CPU *cpu, uint32_t instruction) {
    uint8_t rs1 = RS1(instruction);
    uint8_t rs2 = RS2(instruction);
    uint8_t funct3 = FUNCT3(instruction);
    int32_t imm11_5 = (instruction >> 25) & 0x7F;
    int32_t imm4_0 = (instruction >> 7) & 0x1F;
    int32_t imm = (imm11_5 << 5) | imm4_0;
    imm = sign_extend_to_int(imm, 12);
    
    uint32_t addr = cpu->registers[rs1] + imm;
    uint32_t rs2_val = cpu->registers[rs2];
    if (addr == UART_DATA_REG) {
        printf("%c", (char)(rs2_val & 0xFF));
        fflush(stdout);
        return 0;
    } else if (addr == UART_STATUS_REG) {
        return 0;
    }
    
    if (addr >= cpu->mem_size) {
        ERROR_LOG("Store to invalid memory address: 0x%08X", addr);
        return -1;
    }
    
    switch (funct3) {
        case 0x0:
            cpu->memory[addr] = rs2_val & 0xFF;
            break;
        case 0x1:
            if (addr & 1) {
                ERROR_LOG("Unaligned SH at address: 0x%08X", addr);
                return -1;
            }
            cpu->memory[addr] = rs2_val & 0xFF;
            cpu->memory[addr+1] = (rs2_val >> 8) & 0xFF;
            break;
        case 0x2:
            if (addr & 3) {
                ERROR_LOG("Unaligned SW at address: 0x%08X", addr);
                return -1;
            }
            cpu->memory[addr] = rs2_val & 0xFF;
            cpu->memory[addr+1] = (rs2_val >> 8) & 0xFF;
            cpu->memory[addr+2] = (rs2_val >> 16) & 0xFF;
            cpu->memory[addr+3] = (rs2_val >> 24) & 0xFF;
            break;
        default:
            ERROR_LOG("Invalid store funct3: 0x%X", funct3);
            return -1;
    }
    
    DEBUG_LOG("STORE 0x%08X -> 0x%08X", rs2_val, addr);
    
    return 0;
}

int handle_immediate(CPU *cpu, uint32_t instruction) {
    uint8_t rd = RD(instruction);
    uint8_t rs1 = RS1(instruction);
    uint8_t funct3 = FUNCT3(instruction);
    uint32_t imm11_0 = (instruction >> 20) & 0xFFF;
    
    int32_t imm = sign_extend_to_int(imm11_0, 12);
    uint32_t rs1_val = cpu->registers[rs1];
    uint32_t result = 0;
    
    switch (funct3) {
        case ALU_ADD:
            result = rs1_val + imm;
            break;
        case ALU_SLL:
            result = rs1_val << (imm & 0x1F);
            break;
        case ALU_SLT:
            result = ((int32_t)rs1_val < imm) ? 1 : 0;
            break;
        case ALU_SLTU:
            result = (rs1_val < ((uint32_t)imm)) ? 1 : 0;
            break;
        case ALU_XOR:
            result = rs1_val ^ imm;
            break;
        case ALU_SRL:
            {
                uint8_t funct7 = FUNCT7(instruction);
                if ((funct7 & 0x0F) == 0x5) { // SRLI
                    result = rs1_val >> (imm & 0x1F);
                } else if (funct7 == 0x20) { // SRAI
                    result = ((int32_t)rs1_val) >> (imm & 0x1F);
                } else {
                    ERROR_LOG("Invalid immediate operation");
                    return -1;
                }
            }
            break;
        case ALU_OR:
            result = rs1_val | imm;
            break;
        case ALU_AND:
            result = rs1_val & imm;
            break;
        default:
            ERROR_LOG("Invalid immediate funct3: 0x%X", funct3);
            return -1;
    }
    
    cpu->registers[rd] = result;
    DEBUG_LOG("IMM op x%d <- 0x%08X", rd, result);
    
    return 0;
}

int handle_operation(CPU *cpu, uint32_t instruction) {
    uint8_t rd = RD(instruction);
    uint8_t rs1 = RS1(instruction);
    uint8_t rs2 = RS2(instruction);
    uint8_t funct3 = FUNCT3(instruction);
    uint8_t funct7 = FUNCT7(instruction);
    
    uint32_t rs1_val = cpu->registers[rs1];
    uint32_t rs2_val = cpu->registers[rs2];
    uint32_t result = 0;
    
    switch (funct3) {
        case ALU_ADD:
            if (funct7 == ALU_FUNCT7_ADD) { // ADD
                result = rs1_val + rs2_val;
            } else if (funct7 == ALU_FUNCT7_SUB) { // SUB
                result = rs1_val - rs2_val;
            } else {
                ERROR_LOG("Invalid ADD/SUB funct7: 0x%X", funct7);
                return -1;
            }
            break;
        case ALU_SLL: // SLL
            result = rs1_val << (rs2_val & 0x1F);
            break;
        case ALU_SLT: // SLT
            result = ((int32_t)rs1_val < (int32_t)rs2_val) ? 1 : 0;
            break;
        case ALU_SLTU: // SLTU
            result = (rs1_val < rs2_val) ? 1 : 0;
            break;
        case ALU_XOR: // XOR
            result = rs1_val ^ rs2_val;
            break;
        case ALU_SRL:
            if (funct7 == 0x00) { // SRL
                result = rs1_val >> (rs2_val & 0x1F);
            } else if (funct7 == 0x20) { // SRA
                result = ((int32_t)rs1_val) >> (rs2_val & 0x1F);
            } else {
                ERROR_LOG("Invalid SRL/SRA funct7: 0x%X", funct7);
                return -1;
            }
            break;
        case ALU_OR: // OR
            result = rs1_val | rs2_val;
            break;
        case ALU_AND: // AND
            result = rs1_val & rs2_val;
            break;
        default:
            ERROR_LOG("Invalid operation funct3: 0x%X", funct3);
            return -1;
    }
    
    cpu->registers[rd] = result;
    DEBUG_LOG("OP x%d <- 0x%08X", rd, result);
    
    return 0;
}

int handle_system(CPU *cpu, uint32_t instruction) {
    uint8_t funct3 = FUNCT3(instruction);
    
    if (funct3 == 0x0) {
        uint8_t funct12 = instruction >> 20;
        
        if (funct12 == 0x0) {
            uint32_t syscall_num = cpu->registers[17];
            
            switch (syscall_num) {
                case 93:
                    INFO_LOG("System call EXIT called with arg: %d", cpu->registers[10]);
                    return 1;
                case 0:
                    INFO_LOG("System call EXIT called");
                    return 1;
                default:
                    INFO_LOG("ECALL system call %d not implemented", syscall_num);
                    break;
            }
        } else if (funct12 == 0x1) {
            INFO_LOG("EBREAK encountered at PC 0x%08X", cpu->pc);
            return 1;
        }
    }
    
    return 0;
}




