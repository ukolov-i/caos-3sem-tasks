#!/bin/sh

######## for arm #########
/bin/aarch64-linux-gnu-gcc $(find ./task_04/inf-I-04-3-asm-arm -type f \( -name "*.s" \)) -o ./build/test.out
echo "COMPILED"
qemu-aarch64 -L /usr/aarch64-linux-gnu/ ./build/test.out

######## for intel #########
# gcc -masm=intel -no-pie $(find ./task_05/inf-I-05-2-asm-x86 -type f \( -name "*.s" -o -name "*.c" \)) -o ./build/test.out
# echo "COMPILED"
# ./build/test.out

