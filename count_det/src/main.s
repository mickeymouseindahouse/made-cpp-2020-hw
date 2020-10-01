.text
.global count_det

count_det:

    push {r4-r10}

    ldr r1, [r0], #4
    ldr r2, [r0], #4
    ldr r3, [r0], #4
    ldr r4, [r0], #4
    ldr r5, [r0], #4
    ldr r6, [r0], #4
    ldr r7, [r0], #4
    ldr r8, [r0], #4
    ldr r9, [r0], #4

    mov r0, #0

    mul r0, r5, r9
    mul r10, r6, r8
    sub r0, r10
    mul r1, r0

    mul r0, r6, r7
    mul r10, r4, r9
    sub r0, r10
    mul r2, r0

    mul r0, r4, r8
    mul r10, r5, r7
    sub r0, r10
    mul r3, r0

    add r0, r1, r2
    add r0, r3

    pop {r4-r10}

    bx lr
