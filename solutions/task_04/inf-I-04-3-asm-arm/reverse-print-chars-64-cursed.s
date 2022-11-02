.global main
main:
    stp     x29, x30, [sp, -24]!
    mov     x29, sp
    str     w0, [sp, 20]
    mov     w0, 0
    add     x1, sp, 20
    mov     w2, 1
    bl      read
    cbnz    w0, .CondSecondBranch_main

.CondFristBranch_main:
    add     x1, sp, 20
    bl      write
    ldp     x29, x30, [sp], 24
    ret

.CondSecondBranch_main:
    bl      main
    cmp     w0, 0
    cset    w0, ne
    b       .CondFristBranch_main
