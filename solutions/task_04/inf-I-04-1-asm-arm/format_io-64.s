.extern printf
.extern scanf

.LInput:
        .string "%d%d"
.LOutput:
        .string "%d"

.text
.globl main

main:
        // i dunno how to get address of register
        // and why we want two words
        stp     x29, x30, [sp, -32]!
        mov     x29, sp
        // two pointers on stack
        add     x2, sp, 24
        add     x1, sp, 28
//        mov     x3, 0
//        mov     x4, 0
//        ldr     x1,= x3
//        ldr     x2,= x4
        // lower 12 bits dunno why
        adrp    x0, .LInput
        add     x0, x0, :lo12:.LInput
        bl      scanf
//        mov     x3, 1
//        mov     x4, 2        
//        str     x3, [x1]
//        str     w0, [x2]
        // add scanned values
        ldr     w1, [sp, 28]
        ldr     w0, [sp, 24]
        add     w1, w1, w0

        adrp    x0, .LOutput
        add     x0, x0, :lo12:.LOutput
        bl      printf
        // return code 0 and free stack
        mov     w0, 0
        ldp     x29, x30, [sp], 32
        ret
