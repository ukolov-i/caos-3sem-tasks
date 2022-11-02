.extern getchar
.extern putchar

.text
.global main


main:
        stp     x29, x30, [sp, -32]!
        mov     x29, sp

        mov     w1, 0
        b       .LoopBegin
    
.LoopBegin:
        bl      getchar
        str     w0, [sp, 28]
        cmn     w0, 1
        mov     w1, w0
        bne     .CheckBounds
        b       .LoopEnd

.CheckBounds:
        mov     w0, w1
        cmp     w0, '0'
        blt     .LoopBegin
        cmp     w0, '9'
        bgt     .LoopBegin

        bl      putchar
        b       .LoopBegin

.LoopEnd:
        mov     w0, 0
        ldp     x29, x30, [sp], 32

        ret
