.section .entry
.global _start
_start:
    call user_main
    j _start
