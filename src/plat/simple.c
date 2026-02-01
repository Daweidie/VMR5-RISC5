#include <stdio.h>
#include <stdint.h>
uint8_t platform_read_byte(uint32_t addr) {
    (void)addr;
    return 0;
}
void platform_write_byte(uint32_t addr, uint8_t value) {
    if (addr == 0x10000000) {
        putchar(value);
        fflush(stdout);
    } else if (addr == 0x10000004) {
    }
    (void)addr;
    (void)value;
}
/*
这段C代码实现了一个平台字节写入函数
当写入地址为0x10000000时
将值输出到控制台并刷新缓冲区
当地址为0x10000004时无操作
其他地址也无操作
函数通过(void)强制使用参数避免编译警告。
*/
int platform_init() {
    printf("Platform initialized\n");
    return 0;
}
void platform_cleanup() {
    printf("Platform cleanup\n");
}



