    .intel_syntax noprefix
    .text
    .global sum


//
//      c-like signature
//      sum(int N, const int* A, const int* B, int* R)
//
//      edi = N
//      esi = A
//      edx = B
//      rcx = R
//

sum:
    test    rdi, rdi                        # N != 0
    je      .LoopEnd

.LoopBegin:
    mov     eax, 0                          # eax = i = 0
    sal     rdi, 2                          # N *= 4
    jmp     .LoopBody

.LoopBody:
    mov     r8d, DWORD PTR [rdx + rax]      # r8d = B[i]
    add     r8d, DWORD PTR [rsi + rax]      # r8d += A[i]
    mov     DWORD PTR [rcx + rax], r8d      # R[i] = r8d
    
    add     rax, 4                          # i += 4
    cmp     rdi, rax                        # i != N
    jne     .LoopBody
    jmp     .LoopEnd

.LoopEnd:
    ret
