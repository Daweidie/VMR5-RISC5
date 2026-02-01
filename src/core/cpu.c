#include <stdlib.h>
#include <string.h>
#include "../../include/cpu.h"
#include "../../include/log.h"

CPU* cpu_init(void) {
    CPU *cpu = malloc(sizeof(CPU));//为cpu分配内存
    if (!cpu) {
        ERROR_LOG("Failed to allocate memory for CPU");//分配失败
        return NULL;//返回空指针
    }
    cpu->memory = malloc(MEM_SIZE);//为cpu的内存空间分配内存mem_size大小
    if (!cpu->memory) {
        ERROR_LOG("Failed to allocate memory for CPU memory space");//分配失败
        free(cpu);//释放cpu
        return NULL;
    }

    cpu->mem_size = MEM_SIZE;
    cpu_reset(cpu);

    INFO_LOG("CPU initialized successfully");//初始化成功日志
    return cpu;
}

void cpu_free(CPU *cpu) {
    if (cpu) {
        if (cpu->memory) {
            free(cpu->memory);
        }
        free(cpu);
    }
}//释放cpu结构体的内存，先检查cpu指令是否有效，再释放cpu结构体的内存

void cpu_reset(CPU *cpu) {//复位cpu
    for (int i = 0; i < NUM_REGISTERS; i++) {
        cpu->registers[i] = 0;//清空所有的寄存器
    }
    cpu->registers[0] = 0;//寄存器0始终为0
    cpu->pc = 0x0;//程序计数器复位为0
    memset(cpu->memory, 0, cpu->mem_size);//清空内存
    DEBUG_LOG("CPU reset completed");//复位完成日志
}

void cpu_load_program(CPU *cpu, uint8_t *program, uint32_t size, uint32_t addr) {
    if (addr + size > cpu->mem_size) {//检查程序是否能放入内存
        ERROR_LOG("Program does not fit in memory");//日志报错
        return;//直接返回
    }

    memcpy(cpu->memory + addr, program, size);
    cpu->pc = addr;//设置程序计数器
    INFO_LOG("Loaded %u bytes at address 0x%08X", size, addr);//日志记录加载的程序大小和地址
}



