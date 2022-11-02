    .intel_syntax noprefix
    .text
    .global very_important_function

very_important_function:
    vxorps  ymm2, ymm2, ymm2
    mov     r8, rdi
    mov     r9, rdx
    jmp     .Loop

.Loop:
    sub     rdi, 8
    mov     rax, 4
    mul     rdi

    vmovaps ymm0, [rsi + rax]
    vmovaps ymm1, [r9 + rax]
    vaddps  ymm0, ymm0, ymm1

    vmovaps [rcx + rax], ymm0
    vaddps  ymm2, ymm2, ymm0
    cmp     rdi, 0
    jg      .Loop

    mov     rcx, 8
    vxorps  ymm0, ymm0, ymm0

.LoopForAverage:
    vextractf128    xmm1, ymm2, 0
    vaddps          xmm0, xmm0, xmm1
    vpermilps       ymm3, ymm2, 0b0111001
    vperm2f128      ymm4, ymm3, ymm3, 0b10000001
    vblendps        ymm2, ymm3, ymm4, 0b10001000
    loop            .LoopForAverage

    vcvtsi2sd       xmm1, xmm1, r8
    vcvtss2sd       xmm0, xmm0, xmm0
    vdivsd          xmm0, xmm0, xmm1
    ret
