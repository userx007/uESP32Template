cmake_minimum_required(VERSION 3.16)
project(startup ASM)

if(TARGET_PLATFORM STREQUAL "ESP32")

    add_library(${PROJECT_NAME} STATIC src/startup_esp32.S )

elseif(TARGET_PLATFORM STREQUAL "ESP8266")

    add_library(${PROJECT_NAME} STATIC src/startup_esp8266.S )

endif()



