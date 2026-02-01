riscv64-unknown-elf-as -march=rv32i -mabi=ilp32 --defsym=__riscv_xlen=32 test.asm -o test.o
riscv64-unknown-elf-ld -m elf32lriscv -Ttext=0x80000000 test.o -o test.elf
riscv64-unknown-elf-objcopy -O binary test.elf test.bin
rm test.o test.elf