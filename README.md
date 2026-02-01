========================VMR5=========================



include/: 存放头文件，包括 cpu.h、log.h、opcodes.h 和 risc5.h
src/: 存放源代码
core/: 核心模拟器代码 (cpu.c、risc5.c、opcodes.c)
utils/: 工具函数 (log.c)
plat/: 平台相关代码 (simple.c)
main.c: 程序入口点


features:


CPU 模拟: 实现了具有 32 个寄存器和内存空间的虚拟 CPU

指令解码与执行: 支持多种 RISC-V 指令类型，如：

LUI/AUIPC (加载立即数)

JAL/JALR (跳转和链接)

分支指令 (BEQ, BNE, BLT, BGE 等)

LOAD/STORE (内存访问)

立即数运算 (ADDI, XORI 等)

算术逻辑运算 (ADD, SUB, AND, OR 等)

系统调用 (ECALL, EBREAK)

内存管理: 实现了 1MB 的虚拟内存空间

外设模拟: 包含 UART 串口输出功能，地址 0x10000000 用于输出字符到控制台

可以通过 make 命令构建项目，然后运行 ./vmr5 [program_file] 来执行 RISC-V 二进制程序 
如果没有指定程序文件 
则会启动一个空的 cpu

在根目录有一个test.bin 文件,输出"Hello VMR5",可以作为测试用例
