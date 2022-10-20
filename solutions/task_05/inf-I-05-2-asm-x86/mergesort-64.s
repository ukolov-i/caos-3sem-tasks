    .intel_syntax noprefix
    .text
    .global mergesort


//
//      c-like signature
//      void
//      insert_dispatch(int left, int size, const int* in, int* out)
//
//      edi = left
//      esi = size
//      rdx = in
//      rcx = out
//
//      inserts $size of elements of $in starting from $left to $out
//

insert_dispatch:
    movsx   rdi, edi                        # rdi = left
    movsx   rsi, esi                        # rsi = size
    jmp     .LoopBegin_insert_dispatch

.LoopBegin_insert_dispatch:
    // loop variables initialization
    mov     eax, 0                          # eax = i = 0
    sal     rsi, 2                          # size *= 4
    lea     rdi, [rdx + rdi * 4]            # rdi = in[left] 
    jmp      .LoopBody_insert_dispatch
    
.LoopBody_insert_dispatch:
    mov     edx, DWORD PTR [rdi + rax]      # edx = in[left + i]
    mov     DWORD PTR [rcx + rax], edx      # out[i] = edx

    // iterator incrementing, checking loop condition
    add     rax, 4                          # i += 4
    cmp     rsi, rax                        # i != size
    jne     .LoopBody_insert_dispatch
    jmp     .LoopEnd_insert_dispatch

.LoopEnd_insert_dispatch:
    ret


//
//      c-like signature
//      void
//      merge_helper(int left, int right, int* index, const int* in, int* out)
//
//      edi = left
//      esi = right
//      edx = index
//      rcx = in
//      r8 = out
//
//      writes elements from $left to $right of $in to $out after $index
//

merge_helper:
    mov     r9d, edi                        # r9d = left
    mov     rdi, rdx                        # rdi = index
    jmp     .LoopBegin_merge_helper
    
// used to check input range (note that it CAN be invalid)
.LoopBegin_merge_helper:
    cmp     r9d, esi                        # left >= right
    jge     .LoopEnd_merge_helper
    movsx   r10, r9d                        # r10 = left

    // save index
    mov     eax, DWORD PTR [rdx]            # eax = &index
    
    // now we will iterate using pointers
    lea     rdx, [rcx + r10 * 4]            # rdx = left = in + left
    lea     rsi, [rcx + rsi * 4]            # rsi = right = in + right
    jmp     .LoopBody_merge_helper

.LoopBody_merge_helper:
    mov     ecx, DWORD PTR [rdx]            # ecx = left
    cdqe
    mov     DWORD PTR [r8 + rax * 4], ecx   # out[*index] = ecx

    // index incremention
    mov     eax, DWORD PTR [rdi]            # eax = *index
    add     eax, 1                          # ++eax
    mov     DWORD PTR [rdi], eax            # *index = eax

    // iterator incrementing, checking loop condition
    add     rdx, 4                          # left += 4
    cmp     rdx, rsi                        # left != right
    jne     .LoopBody_merge_helper
    jmp     .LoopEnd_merge_helper

.LoopEnd_merge_helper:
    ret


//
//      c-like signature
//      void
//      merge(int left, int middle, int right, int* out)
//
//      edi = left
//      esi = middle
//      edx = right
//      rcx = out
//
//      merges sorted parts $[left, middle) and $[middle, right) of $out
//

merge:
    mov     r10d, esi                       # r10d = size_left = middle
    sub     r10d, edi                       # size_left -= left
    sub     edx, esi                        # right -= left
    mov     r11d, edx                       # r11d = size_right = right
    mov     r8, rcx                         # r8 = out
    mov     r9d, edi                        # r9d = left
    movsx   rax, r10d                       # rax = size_left

    // push variables on stack and safe stack pointer
    push    rbp
    mov     rbp, rsp
    push    rbx
    push    r12
    push    r13

    // making space for array on stack
    lea     rax, [0xf + rax * 4]            # copy_left[size_left]
    mov     r13d, esi                       # r13d = left
    and     rax, 0xFFFFFFFFFFFFFFF0         # alignment mask
    mov     esi, r10d

    // making space for array on stack
    sub     rsp, 24
    mov     DWORD PTR [rbp - 36], edi       # left put on stack
    sub     rsp, rax                        # rsp -= sizeof(copy_left)
    movsx   rax, edx                        # rax = size_right

    lea     rax, [0xf + rax * 4]            # copy_right[size_right]
    mov     r12, rsp                        # r12 = stack pointer
    and     rax, 0xFFFFFFFFFFFFFFF0         # alignment mask

    // set arguments, call function
    sub     rsp, rax                        # rsp -= sizeof(copy_right)
    mov     rcx, r12                        # rcx = stack pointer
    mov     rdx, r8                         # rdx = out
    call    insert_dispatch

    // set arguments, call function
    mov     rbx, rsp                        # rbx = stack pointer
    mov     edi, r13d                       # edi = middle
    mov     esi, r11d                       # esi = size_right
    mov     rdx, r8                         # rdx = out
    mov     rcx, rbx                        # rcx = stack pointer
    call    insert_dispatch

    lea     eax, [r9 + 1]

    mov     edi, 0                          # edi = i = 0
    mov     r9d, 0                          # r9d = i       
    mov     esi, 0                          # esi = j = 0
    mov     r13d, 0                         # r13d = j
    jmp     .LoopBegin_merge

.LoopBegin_merge:
    mov     ecx, DWORD PTR [r12 + r9 * 4]   # ecx = copy_left[i]
    mov     edx, DWORD PTR [rbx + rsi * 4]  # edx = copy_right[j]
    cmp     edx, ecx                        # copy_right[j] < copy_left[i]
    jl      .CondFirstBranch_merge
    jmp     .LoopBody_merge

.LoopBody_merge:
    add     edi, 1                              # ++i
    mov     DWORD PTR [r8 - 4 + rax * 4], ecx   # out[left] = copy_left[i]
    lea     rdx, [rax + 1]

    cmp     edi, r10d                           # i >= size_left
    jge     .LoopEnd_merge
    cmp     r11d, r13d                          # size_right <= j
    jle     .LoopEnd_merge

    movsx   r9, edi
    mov     rax, rdx

    mov     ecx, DWORD PTR [r12 + r9 * 4]       # ecx = copy_left[i]
    mov     edx, DWORD PTR [rbx + rsi * 4]      # edx = copy_right[j]
    cmp     edx, ecx                            # copy_right[j] < copy_left[i]
    jge     .LoopBody_merge
    jmp     .CondFirstBranch_merge

.CondFirstBranch_merge:
    add     r13d, 1                             # ++j
    mov     DWORD PTR [r8 - 4 + rax * 4], edx   # out[left] = copy_right[j]
    lea     rdx, [rax + 1]            

    cmp     r10d, edi                           # size_left <= i
    jle     .LoopEnd_merge
    cmp     r13d, r11d                          # j >= size_right 
    jge     .LoopEnd_merge

    mov     rax, rdx
    movsx   rsi, r13d

    jmp     .LoopBegin_merge

.LoopEnd_merge:
    mov     DWORD PTR [rbp - 36], eax       

    // setting argumnets and cl=alling function
    mov     esi, r10d                   # esi = size_left
    lea     rdx, [rbp - 36]             # rdx = &left
    mov     rcx, r12
    call    merge_helper

    mov     edi, r13d                   # edi = j
    mov     esi, r11d                   # esi = size_right
    lea     rdx, [rbp - 36]             # rdx = &left
    mov     rcx, rbx
    call    merge_helper

    lea     rsp, [rbp - 24]

    // free stack
    pop     r13
    pop     r12
    pop     rbx
    pop     rbp

    ret


//
//      c-like signature
//      void
//      mergesort_impl(int left, int right, int* out)
//
//      edi = left
//      esi = rigth
//      edx = out
//
//      actual body of algorithm
//

mergesort_impl:
    jmp      .CondBegin_mergesort_impl

.CondBegin_mergesort_impl:
    lea     eax, [rdi + 1]                  # eax = left + 1
    cmp     eax, esi                        # left + 1 < right
    jl      .CondFirstBranch_mergesort_impl                    
    jmp     .CondEnd_mergesort_impl

.CondFirstBranch_mergesort_impl:
    // push variables on stack
    push    rbp
    push    rbx
    push    r12
    push    r13

    lea     r13d, [rdi + rsi]               # r13d = middle = left + right
    sar     r13d                            # middle /= 2

    // storing arguments before calling function
    mov     ebx, edi                        # ebx = left
    mov     ebp, esi                        # ebp = right
    mov     r12, rdx                        # r12 = out

    // setting arguments and calling function
    mov     esi, r13d                       # esi = middle
    call    mergesort_impl

    // setting arguments and calling function
    mov     esi, ebp                        # esi = right
    mov     edi, r13d                       # edi = middle
    mov     rdx, r12                        # rdx = out
    call    mergesort_impl

    // setting arguments and calling function
    mov     edi, ebx                        # edi = left
    mov     esi, r13d                       # esi = middle
    mov     edx, ebp                        # edx = right
    mov     rcx, r12                        # rcx = out

    // free stack
    pop     r13
    pop     r12
    pop     rbx
    pop     rbp

    jmp     merge

.CondEnd_mergesort_impl:
    ret

//
//      c-like signature
//      void
//      mergesort(int from, int to, const int *in, int *out)
//
//      edi = from
//      esi = to
//      edx = in
//      rcx = out
//
//      obvious
//

mergesort:
    sub     esi, edi                        # to -= from
    mov     rax, rdx                        # rax = in
    mov     rdx, rcx                        # rdx = out

    movsx   rcx, esi                        # rcx = to
    movsx   rdi, edi                        # rdi = from
    lea     r8, [rax + rdi * 4]             # r8 = in[from]
    jmp     .LoopBegin_mergesort

.LoopBegin_mergesort:
    sal     rcx, 2                          # to *= 4
    mov     eax, 0                          # eax = i = 0              
    jmp     .LoopBody_mergesort

.LoopBody_mergesort:
    mov     edi, DWORD PTR [r8 + rax]       # edi = in[from + i]
    mov     DWORD PTR [rdx + rax], edi      # out[i] = edi

    // iterator incrementing, checking loop condition
    add     rax, 4                          # i += 4
    cmp     rcx, rax                        # i != to - from
    jne     .LoopBody_mergesort
    jmp     .LoopEnd_mergesort

.LoopEnd_mergesort:
    // seeting arguments, calling function
    mov     edi, 0                          # from = 0
    jmp     mergesort_impl
