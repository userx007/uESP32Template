# toolchain-esp8266.cmake

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR xtensa)

# Platform definition
set(TARGET_PLATFORM "ESP8266" CACHE STRING "Target platform")

set(CMAKE_C_COMPILER xtensa-lx106-elf-gcc)
set(CMAKE_CXX_COMPILER xtensa-lx106-elf-g++)
set(CMAKE_ASM_COMPILER xtensa-lx106-elf-gcc)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# Optional: Set path to your ESP8266 SDK or sysroot
# set(CMAKE_FIND_ROOT_PATH /path/to/esp-open-sdk)

# Optional: generate a compile_commands.json file—handy for tools like clang-tidy or auto-completion in some IDEs
# set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
