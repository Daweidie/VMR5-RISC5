#include <stdio.h>
#include <stdlib.h>
#include "../include/risc5.h"
#include "../include/opcodes.h"
#include "../include/log.h"

int cpu_step(CPU *cpu) {
    if (cpu->pc >= cpu->mem_size) {
        ERROR_LOG("PC out of bounds: 0x%08X", cpu->pc);
        return -1;
    }
    uint32_t instruction = *(uint32_t*)(cpu->memory + cpu->pc);
    DEBUG_LOG("Fetching instruction 0x%08X at PC 0x%08X", instruction, cpu->pc);
    int result = execute_instruction(cpu, instruction);
    if (result == 0) {
        cpu->pc += 4;
    }
    
    return result;
}

void risc5_emulator_run(CPU *cpu) {
    INFO_LOG("Starting RISC-V emulation...");
    
    while (1) {
        int status = cpu_step(cpu);
        if (status != 0) {
            break;
        }
    }
    
    INFO_LOG("Emulation stopped");
}

void risc5_emulator_step(CPU *cpu) {
    cpu_step(cpu);
}

int risc5_emulator_init(void) {
    INFO_LOG("RISC-V emulator initialized");
    return 0;
}




