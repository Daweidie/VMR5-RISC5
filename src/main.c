#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef _WIN32
#else
int optind = 1;
int getopt(int argc, char *argv[], const char *optstring);
#endif
#include "../include/cpu.h"
#include "../include/risc5.h"
#include "../include/log.h"
int main(int argc, char *argv[]) {
    INFO_LOG("Starting RISC-V Emulator");

    if (risc5_emulator_init() != 0) {
        ERROR_LOG("Failed to initialize emulator");
        return -1;
    }
    CPU *cpu = cpu_init();
    if (!cpu) {
        ERROR_LOG("Failed to initialize CPU");
        return -1;
    }
    if (argc > 1) {
        FILE *fp = fopen(argv[1], "rb");
        if (!fp) {
            ERROR_LOG("Cannot open program file: %s", argv[1]);
            cpu_free(cpu);
            return -1;
        }
        fseek(fp, 0, SEEK_END);
        long fsize = ftell(fp);
        fseek(fp, 0, SEEK_SET);
        uint8_t *program = malloc(fsize);
        if (!program) {
            ERROR_LOG("Cannot allocate memory for program");
            fclose(fp);
            cpu_free(cpu);
            return -1;
        }

        fread(program, 1, fsize, fp);
        fclose(fp);
        cpu_load_program(cpu, program, fsize, 0x0);

        free(program);
    } else {
        INFO_LOG("No program file specified, starting with empty memory");
    }
    INFO_LOG("Starting emulation...");
    risc5_emulator_run(cpu);
    cpu_free(cpu);
    INFO_LOG("Emulator finished");

    return 0;
}


