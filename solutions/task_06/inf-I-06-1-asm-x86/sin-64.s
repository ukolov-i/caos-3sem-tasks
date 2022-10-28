    .intel_syntax noprefix
    .text
    .global avx_sin


//
//      c-like signature
//      double
//      avx_sin(double x);
//

avx_sin:
    push        rbp
    mov         rbp, rsp

    vxorps      xmm5, xmm5, xmm5            # xmm5 = res = 0
    mov         rax, 1
//    vmovd       xmm1, rax
    vxorps      xmm7, xmm7, xmm7            # xmm7 = 0
    vcvtsi2sd   xmm8, xmm8, rax             # xmm8 = 1
    vcvtsi2sd   xmm1, xmm1, rax             # xmm1 = term = 1
    vcvtsi2sd   xmm3, xmm3, rax             # xmm3 = fact = 1
    vcvtsi2sd   xmm6, xmm6, rax             # xmm6 = flag = 1

    vmovsd      xmm2, xmm2, xmm0            # xmm2 = power = x
    jmp         .LoopBody_avx_sin

.LoopBody_avx_sin:
    # xmm4 = xmm2/xmm3
    vmovsd      xmm4, xmm4, xmm2            # xmm4 = power
    vdivsd      xmm4, xmm4, xmm3            # power /= fact

    vcomisd     xmm4, xmm7                  # power == 0
//    vtestpd     xmm4, xmm4                  # power == 0
    je          .LoopEnd_avx_sin

    vcomisd     xmm6, xmm7                  # flag == 0
//    vtestps     xmm6, xmm6 
    je          .CondFirstBranch_avx_sin
    jmp         .CondSecondBranch_avx_sin

.CondFirstBranch_avx_sin:
    vmovsd      xmm6, xmm6, xmm8            # flag = 1
    vsubsd      xmm5, xmm5, xmm4            # res -= term
    jmp         .CondEnd_avx_sin

.CondSecondBranch_avx_sin:
    vmovsd      xmm6, xmm6, xmm7            # flag = 0
    vaddsd      xmm5, xmm5, xmm4            # res += term
    jmp         .CondEnd_avx_sin

.CondEnd_avx_sin:
    // lets hope that first 12 terms are enough
    vaddsd      xmm1, xmm1, xmm8            # ++pos
    vmulsd      xmm3, xmm3, xmm1            # fact *= pos 
    vaddsd      xmm1, xmm1, xmm8            # ++pos
    vmulsd      xmm3, xmm3, xmm1            # fact *= pos
    

    vmulsd      xmm2, xmm2, xmm0            #  power *= x
    vmulsd      xmm2, xmm2, xmm0            #  power *= x
    jmp         .LoopBody_avx_sin

.LoopEnd_avx_sin:
    vmovsd      xmm0, xmm0, xmm5

    pop rbp
    ret
