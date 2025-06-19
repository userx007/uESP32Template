
cmake_minimum_required(VERSION 3.16)
project(uart_access)


add_library(${PROJECT_NAME}
    STATIC
        src/uart_access.cpp
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${PROJECT_SOURCE_DIR}/inc
)

