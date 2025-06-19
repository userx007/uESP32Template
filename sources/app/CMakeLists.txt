
cmake_minimum_required(VERSION 3.10)
project(espshell)

add_executable(${PROJECT_NAME}
        src/main.cpp
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        startup
        uart_access
        ushell_core
        ushell_core_utils
        ushell_user_root
)

