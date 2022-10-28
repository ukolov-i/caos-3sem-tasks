    .intel_syntax noprefix
    .text
    .global dot_product


//
//      c-like signature
//      float
//      dot_product(size_t N, const float *A, const float *B)
//
//      edi = N
//      esi = A
//      rdx = B
//

dot_product:
    vxorps  xmm1, xmm1, xmm1                    # result_pack = 0
    jmp     .LoopBegin_dot_product

.LoopBegin_dot_product:
    xor     eax, eax                            # eax = i = 0
    sal     rdi, 2                              # N *= 4

.LoopBody_dot_product:
    vmovss  xmm0, DWORD PTR [rsi + rax]         # value = vector from A
    vmulss  xmm0, xmm0, DWORD PTR [rdx + rax]   # value *= vector from B
    vaddss  xmm1, xmm1, xmm0                    # result += value             

    add     rax, 4                              # i += 4
    cmp     rdi, rax                            # i != N
    jne     .LoopBody_dot_product
    jmp     .LoopEnd_dot_product

.LoopEnd_dot_product:
    vmovaps xmm0, xmm1                          # return result
    ret
