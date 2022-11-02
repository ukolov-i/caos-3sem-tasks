        .extern printf
        .extern scanf

        .text
        .global main

main:
        stp     x29, x30, [sp, -32]!
        mov     x29, sp

        ldr     x0, =.LInputFormat
        add     x1, sp, 24
        add     x2, sp, 28
        bl      scanf

        ldp     w1, w0, [sp, 24]
        add     w1, w1, w0

        ldr     x0, =.LOutputFormat
        bl      printf   

        mov     w0, 0
        ldp     x29, x30, [sp], 32
        ret


        .data
.LInputFormat:
        .string "%d%d"
.LOutputFormat:
        .string "%d"