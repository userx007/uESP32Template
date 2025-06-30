cmake_minimum_required(VERSION 3.16)

# Let ESP-IDF know where your extra components are
set(EXTRA_COMPONENT_DIRS
    sources/app
    sources/libs/uart_access
    sources/ushell/ushell_core/ushell_core
    sources/ushell/ushell_core/ushell_core_utils
    sources/ushell/ushell_core/ushell_core_config
    sources/ushell/ushell_user/ushell_user_root
    sources/ushell/ushell_settings
)

# Include the ESP-IDF build system (this defines the actual project)
include($ENV{IDF_PATH}/tools/cmake/project.cmake)

project(espshell)
