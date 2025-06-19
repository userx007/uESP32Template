ENTRY(_start)

MEMORY
{
  IRAM (rx)  : ORIGIN = 0x40080000, LENGTH = 0x20000  /* 128KB Instruction RAM */
  DRAM (rw)  : ORIGIN = 0x3FFB0000, LENGTH = 0x20000  /* 128KB Data RAM */
}

SECTIONS
{
  /* Startup code and vector table */
  .entry : ALIGN(4) {
    KEEP(*(.entry))
  } > IRAM

  /* Xtensa literal pool (for l32r) */
  .entry.literal : ALIGN(4) {
    *(.entry.literal)
  } > IRAM

  /* General code */
  .text : ALIGN(16) {
    *(.text*)
  } > IRAM

  /* Literal constants */
  .literal : ALIGN(16) {
    *(.literal*)
    *(.lit4*)
  } > IRAM

  /* Read-only data (strings, constants) */
  .rodata : ALIGN(16) {
    *(.rodata*)
  } > IRAM

  /* Initialized global/static data */
  .data : ALIGN(4) {
    *(.data*)
  } > DRAM AT > IRAM

  /* Uninitialized data (cleared manually at boot) */
  .bss (NOLOAD) : ALIGN(4) {
    _bss_start = .;
    *(.bss*)
    *(COMMON)
    _bss_end = .;
  } > DRAM

  /* Define symbols visible to assembly */
  PROVIDE(_stack_top = ORIGIN(DRAM) + LENGTH(DRAM));
  PROVIDE(_vector_table = ADDR(.entry));


/* Discard unnecessary metadata sections */
DISCARD :
{
  *(.comment)
  *(.note*)
  *(.eh_frame*)
  *(.ctors)
  *(.dtors)
}
}