    .text
    .global solve

solve:
    mov w4, 0
    b LoopBegin

LoopBegin:
    cmp w4, 255
    bge LoopEnd

    // a
    mov w5, w0

    // ax + b
    mul w5, w5, w4
    add w5, w5, w1

    // (ax + b)x + c
    mul w5, w5, w4
    add w5, w5, w2
    
    // (ax^2 + bx + c)x + d
    mul w5, w5, w4
    add w5, w5, w3

    cmp w5, 0
    beq LoopEnd

    add w4, w4, 1
    b LoopBegin

LoopEnd:
    mov x0, x4
    ret
