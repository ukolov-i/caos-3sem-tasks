.extern getchar
.extern putchar

.text
.global main


main:
        // allocating space for temp values
        stp     x29, x30, [sp, -32]!
        mov     x29, sp
        mov     w1, 0
        b       .LoopBegin
    
.LoopBegin:
        bl      getchar
        // i dunno how to create first temp value witjout stack use (((
        str     w0, [sp, 28]
        ldr     w0, [sp, 28]
        // because we actually want to work with chars
        // usual comparison might not be correct
        cmn     w0, 1
        mov     w1, w0
        // if it is not the end of the file, check if char is a digit 
        bne     .CheckBounds
        b       .LoopEnd

.CheckBounds:
        // without proper updating after every operation it does not work
        // dunno why, probably because it is a char
        // bigger than '0'
        mov     w0, w1
        cmp     w0, 47
        ble     .LoopBegin

        // less than '9'
        mov     w0, w1
        cmp     w0, 57
        bgt     .LoopBegin

        // print
        mov     w0, w1
        bl      putchar
        b       .LoopBegin

.LoopEnd:
        // set return code to zero and free stack
        mov     w0, 0
        ldp     x29, x30, [sp], 32
        ret
