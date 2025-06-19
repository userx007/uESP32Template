# uESP32Template


#  Xtensa GCC toolchain

## 1. Download the Prebuilt Toolchain

Espressif provides precompiled Xtensa GCC toolchains for different platforms:


    ESP32 (Xtensa LX6):     https://docs.espressif.com/projects/esp-idf/en/release-v3.0/get-started/linux-setup.html
                            --> xtensa-esp32-elf-linux64-1.22.0-80-g6c4433a-5.2.0.tar.gz

    ESP8266 (Xtensa LX106): https://docs.espressif.com/projects/esp8266-rtos-sdk/en/latest/get-started/linux-setup.html
                            --> xtensa-lx106-elf-gcc8_4_0-esp-2020r3-linux-amd64.tar

    Choose the version for your OS (Linux/macOS/Windows), and download the .tar.gz or .zip archive.

## 2. Extract and Install

    Let’s say you’re on Linux:
    mkdir -p ~/esp
    cd ~/esp
    tar -xzf ~/Downloads/xtensa-esp32-elf-gcc*.tar.gz

    This will create a folder like xtensa-esp32-elf/ inside ~/esp.

##  3. Add to PATH

To make the toolchain available globally:

    echo 'export PATH="$HOME/esp/xtensa-esp32-elf/bin:$PATH"' >> ~/.bashrc
    source ~/.bashrc

Or for one-time use:

    export PATH="$HOME/esp/xtensa-esp32-elf/bin:$PATH"

## 4. Verify Installation

    xtensa-esp32-elf-gcc --version

## Test

    Here's a minimal "Hello, UART" project for the ESP32 using just the Xtensa GCC toolchain and CMake—no ESP-IDF required.
    It sends a simple message over UART0 (the default serial console).

### Project structure

    hello_uart/
    ├── CMakeLists.txt
    ├── main.c
    ├── linker.ld
    ├── startup.S
    └── toolchain.cmake

### main.c

    #include <stdint.h>

    #define UART_FIFO_REG  (*(volatile uint32_t *)0x3FF40000)
    #define UART_TXD_READY (*(volatile uint32_t *)0x3FF40018)
    #define UART_RXD_READY (*(volatile uint32_t *)0x3FF4001C)
    #define UART_RX_FIFO   (*(volatile uint32_t *)0x3FF40078)

    char uart_getchar() {
        // Wait until RX FIFO has data
        while (!(UART_RXD_READY & (1 << 0))) { }
        return UART_RX_FIFO & 0xFF;
    }


    void uart_send_char(char c) {
        while (!(UART_TXD_READY & (1 << 1))) { }
        UART_FIFO_REG = c;
    }

    void uart_send_string(const char *s) {
        while (*s) uart_send_char(*s++);
    }

    void app_main(void) {
        uart_send_string("Hello, UART from bare-metal ESP32!\r\n");
        while (1) { }
    }

### startup.S

    .section .entry
    .global _start
    _start:
        call app_main
        j _start

### linker.ld

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

### CMakeLists.txt

    cmake_minimum_required(VERSION 3.5)
    project(hello_uart C ASM)

    set(CMAKE_SYSTEM_NAME Generic)
    set(CMAKE_SYSTEM_PROCESSOR xtensa)

    set(CMAKE_C_COMPILER xtensa-esp32-elf-gcc)
    set(CMAKE_ASM_COMPILER xtensa-esp32-elf-gcc)

    add_executable(hello_uart.elf main.c startup.S)
    set_target_properties(hello_uart.elf PROPERTIES LINK_FLAGS "-nostdlib -Tlinker.ld")

### toolchain.cmake

    set(CMAKE_SYSTEM_NAME Generic)
    set(CMAKE_SYSTEM_PROCESSOR xtensa)
    set(CMAKE_C_COMPILER xtensa-esp32-elf-gcc)
    set(CMAKE_ASM_COMPILER xtensa-esp32-elf-gcc)


### Build & Flash: Bash

    cmake -Bbuild -H. -DCMAKE_TOOLCHAIN_FILE=toolchain.cmake
    cmake --build build

## Extend

    https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-guides/linker-script-generation.html

