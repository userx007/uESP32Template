set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR xtensa)

# Platform definition
set(TARGET_PLATFORM "ESP32" CACHE STRING "Target platform")

# Ensure C++11 is enabled globally
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Toolchain prefix
set(ESP32_TOOLCHAIN_PREFIX xtensa-esp32-elf)

# Compilers
set(CMAKE_C_COMPILER ${ESP32_TOOLCHAIN_PREFIX}-gcc)
set(CMAKE_CXX_COMPILER ${ESP32_TOOLCHAIN_PREFIX}-g++)
set(CMAKE_ASM_COMPILER ${ESP32_TOOLCHAIN_PREFIX}-gcc)

# Optional: GDB for debugging
set(CMAKE_GDB ${TOOLCHAIN_PREFIX}-gdb)

# Flags
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -nostdlib -Wl,-T${CMAKE_SOURCE_DIR}/linker/linker.ld" CACHE STRING "Linker flags")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-Map=link.map -Wl,--cref -Wl,--print-memory-usage")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-inline -mlongcalls -g0")



# Don't look for standard system libraries
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

# Don't try to compile a test program
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# Optional: can help to explicitly tell CMake where to look for libraries and headers
# set(CMAKE_FIND_ROOT_PATH /path/to/esp32/sysroot)

# Optional: generate a compile_commands.json file—handy for tools like clang-tidy or auto-completion in some IDEs
# set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
