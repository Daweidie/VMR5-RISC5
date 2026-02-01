.text
.globl _start
_start:
    la t0, msg          # t0 = 字符串地址
    li t1, 0x10000000   # t1 = UART设备地址
print_loop:
    lbu t2, 0(t0)       # 加载一个字符
    beqz t2, exit_prog  # 如果是0 就跳转到退出
    sw t2, 0(t1)        # 发送字符
    addi t0, t0, 1      # 指向下一个字符
    j print_loop        # 继续循环

exit_prog:
    li a0, 0            # 设置退出码为 0 (表示成功)
    li a7, 93           # 设置系统调用号: 93 (exit)
    ecall

.data
msg:
    .ascii "Hello VMR5!\n\0"


