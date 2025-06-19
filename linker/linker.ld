ENTRY(_start)

MEMORY
{
  IRAM (rx)  : ORIGIN = 0x40080000, LENGTH = 0x20000
  DRAM (rw)  : ORIGIN = 0x3FFB0000, LENGTH = 0x20000
}

SECTIONS
{
  .text : {
    *(.entry)
    *(.text*)
    *(.rodata*)
  } > IRAM

  .data : {
    *(.data*)
    *(.bss*)
  } > DRAM
}
