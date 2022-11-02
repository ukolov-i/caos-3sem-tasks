    .intel_syntax noprefix
    .text
    .global very_important_function
    .extern A

very_important_function:
    push rbp
    mov rbp, rsp

    push r12
    push r13
    push r14
    sub rsp, 0x8
    mov r13, A
    mov r12, rdi

LoopBegin:
    mov rdi, offset format_scanf
    mov rsi, rsp
    call scanf

    movsxd rax, [rsp]
    movsxd r14, [r13]
    imul rax, r14
    add r13, 4

    mov rdi, offset format_printf
    mov rsi, rax
    call printf

    dec r12
    jnz LoopBegin

    mov rsp, rbp
    pop rbp
    ret

    .section .rodata

format_scanf:
    .string "%d"
format_printf:
    .string "%lld\n"

