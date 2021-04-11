cmake_minimum_required(VERSION 3.13)

# initialize pico-sdk from GIT
# (note this can come from environment, CMake cache etc)
set(PICO_SDK_FETCH_FROM_GIT on)

# pico_sdk_import.cmake is a single file copied from this SDK
# note: this must happen before project()
include(pico_sdk_import.cmake)

project(pico_clock)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

# rest of your project
add_executable(${PROJECT_NAME} main.cpp util/DateHandler.cpp util/DateHandler.h RPi-Pico-SSD1306-library/GFX.hpp RPi-Pico-SSD1306-library/GFX.cpp RPi-Pico-SSD1306-library/SSD1306.hpp RPi-Pico-SSD1306-library/SSD1306.cpp util/ModeHandler.cpp util/ModeHandler.h mode/DateTimeMode.h)
target_link_libraries(${PROJECT_NAME} pico_stdlib hardware_rtc hardware_i2c)
pico_add_extra_outputs(${PROJECT_NAME})
pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)