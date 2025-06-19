.section .entry, "ax"
.global _start

_start:
    // Set up stack pointer
    l32r a1, _stack_top

    // Optionally set vector table base
    l32r a2, _vector_table
    wsr  a2, vecbase
    rsync

    // Zero the .bss
    l32r a2, _bss_start
    l32r a3, _bss_end
bss_clear:
    beq a2, a3, bss_done
    s32i.n a0, a2, 0
    addi a2, a2, 4
    j bss_clear
bss_done:

    // Call main
    call0 user_main

hang:
    j hang
