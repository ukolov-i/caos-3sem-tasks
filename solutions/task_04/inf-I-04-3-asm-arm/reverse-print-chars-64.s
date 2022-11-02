.extern getchar
.extern putchar
.extern malloc
.extern realloc

.text
.global main


//
//      c-like signature
//      int* 
//      init(int capacity)
//
//      creates vector of chars
//

init:
        stp     x29, x30, [sp, -16]!        // take stack
        mov     x29, sp                     

        sbfiz   x0, x0, 2, 32               // capacity *= sizeof(int)
        bl      malloc                      // vector = malloc(capacity)
        cbz     x0, .BadAlloc_init          // vector == 0

        ldp     x29, x30, [sp], 16          // free stack
        ret                                 // return vector

.BadAlloc_init:
        mov     w0, 1                       // exit code = 1
        bl      exit


//
//      c-like signature
//      void
//      expand(int** vector, int* capacity)
//
//      increases $capacity of $vector in 2 times
//

expand:
        stp     x29, x30, [sp, -48]!
        mov     x29, sp

        // save used registers on stack
        stp     x19, x20, [sp, 16]
        str     x21, [sp, 32]
        mov     x19, x0                     // x19 = vector     
        mov     x20, x1                     // x20 = capacity
        ldr     w21, [x1]                   // w21 = new_capacity = capacity
        lsl     w21, w21, 1                 // new_capacity *= 2

        // setting arguments and calling functio
        ldr     x0, [x0]                    // x0 = vector
        sbfiz   x1, x21, 2, 32              // w1 = new_capacity * 4
        bl      realloc                     
        
        // check if realloc was successeful
        cbz     x0, .BadAlloc_expand        // new_vector = realloc(...) != 0

        str     x0, [x19]                   // *vector = new vector
        str     w21, [x20]                  // *capacity = new_capacity

        // load used registers from stack and free stack
        ldp     x19, x20, [sp, 16]
        ldr     x21, [sp, 32]
        ldp     x29, x30, [sp], 48

        ret

.BadAlloc_expand:
        ldr     x0, [x19]                     // x0 = *vector
        bl      free
        b       .BadAlloc_init

//
//      c-like signature
//      void 
//      push_back(int** vector, int* size, int* capacity, const int value)
//
//      inserts $value at end of the $vector
//

push_back:
        stp     x29, x30, [sp, -48]!
        mov     x29, sp

        // save used registers on stack
        stp     x19, x20, [sp, 16]
        str     x21, [sp, 32]

        mov     x19, x1                    // x19 = size
        mov     w20, w3                    // w20 = value
        mov     x21, x0                    // x21 = vector

        ldr     w1, [x1]                   // w1 = *size
        ldr     w3, [x2]                   // w3 = *capacity
        cmp     w3, w1                     // *size != *capacity
        beq     .CondFristBranch_push_back
        b       .CondEnd_push_back


.CondFristBranch_push_back:
        mov     x1, x2                      // x1 = capacity
        bl      expand
        ldr     w1, [x19]                   // w1 = *size
        b       .CondEnd_push_back

.CondEnd_push_back:
        ldr     x0, [x21]                   // x0 = *vector
        ldr     x21, [sp, 32]
        str     w20, [x0, w1, sxtw 2]       // (*vector)[*size] = value

        ldr     w0, [x19]                   // w0 = *size
        add     w0, w0, 1                   // ++w0
        str     w0, [x19]                   // *size = w0

        ldp     x19, x20, [sp, 16]              
        ldp     x29, x30, [sp], 48

        ret


//
//      c-like signature
//      void 
//      print_reverse(const int* vector, int size)
//
//      prints $vector of int with $size in reverse order
//

print_reverse:
        subs    w1, w1, 1                  // --size
        bmi     .LoopEnd_print_reverse      // size < 0   

        stp     x29, x30, [sp, -32]!    
        mov     x29, sp

        // save used registers on stack
        stp     x19, x20, [sp, 16]          // x19, x20 on stack
        mov     x20, x0                     // x20 = vector
        sxtw    x19, w1                     // x19 = size
        b       .LoopBody_print_reverse

.LoopBody_print_reverse:   
        // set arguments and call function
        ldr     w0, [x20, x19, lsl 2]               // w0 = vector[size]
        bl      putchar                 
        sub     x19, x19, 1                         // --size 
        tbz     w19, 31, .LoopBody_print_reverse    // size < 0            

        // load used registers from stack and free stack
        ldp     x19, x20, [sp, 16]
        ldp     x29, x30, [sp], 32
        ret

.LoopEnd_print_reverse:
        ret

main:
        stp     x29, x30, [sp, -48]!
        mov     x29, sp

        // save used registers on stack
        str     x19, [sp, 16]               // x19 on stack

        mov     w0, 8                       // w0 = capacity = 8
        stp     w0, wzr, [sp, 32]           // capacity on stack
        bl      init

        str     x0, [sp, 40]                // vector on stack
        b       .LoopBegin_main

.LoopBegin_main:
        bl      getchar
        mov     w3, w0                      // w3 = getchar()
        add     x2, sp, 32                  // x2 = &capacity
        cmn     w0, #1                      // c != EOF
        bne     .LoopBody_main
        b       .LoopEnd_main

.LoopBody_main:
        add     x0, sp, 40                  // x0 = &vector
        add     x1, sp, 36                  // x1 = &size                    
        bl      push_back
        b       .LoopBegin_main
        
.LoopEnd_main:
        // storing arguments before calling function
        ldr     x19, [sp, 40]               // x19 = vector

        // setting arguments and calling function
        ldr     w1, [sp, 36]                // w1 = size
        mov     x0, x19                     // x0 = vector
        bl      print_reverse

        // setting arguments and calling function
        mov     x0, x19                     // x0 = vector
        bl      free

        // load used registers from stack and free stack
        ldr     x19, [sp, 16]
        ldp     x29, x30, [sp], 48

        mov     w0, 0                       // exit code = 0
        ret
