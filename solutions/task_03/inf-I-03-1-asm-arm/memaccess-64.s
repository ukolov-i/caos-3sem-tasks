    .text
    .global sum

sum:
    mov x3, 0
    mov w4, 0
    b LoopBegin

LoopBegin:
    cmp x3, x1
    bge LoopEnd
    ldr w4, [x2, x3, lsl 2]
    add w0, w0, w4
    add x3, x3, 1
    b LoopBegin

LoopEnd:
    ret
