    .text

    .global fib

fib:
    # n: a0
    
    # Reserve stack space
    # ra -> 16(sp)
    # a0 -> 8(sp)
    # t0 -> 0(sp)
    addi sp, sp, -24
    
    # push ra
    sd ra, 16(sp)

    # Base case: n < 1 -> return 0
    li t0, 1
    blt a0, t0, fib_exit_zero

    # Base case: n = 1 -> return 1
    ble a0, t0, fib_exit_one


    # fib(n - 1)
    sd a0, 8(sp)
    addi a0, a0, -1
    jal fib
    mv t0, a0
    ld a0, 8(sp)

    
    # fib(n - 2)
    sd t0, 0(sp)
    addi a0, a0, -2
    jal fib
    ld t0, 0(sp)

    # Sum
    add a0, t0, a0

    j fib_exit

fib_exit_one:
    li a0, 1
    j fib_exit

fib_exit_zero:
    li a0, 0

fib_exit:

    # pop ra
    ld ra, 16(sp)

    addi sp, sp, 24

    ret



