cmake_minimum_required(VERSION 3.12)

include(pico_sdk_import.cmake)

project(PICO_MQTT C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

#Caso seja a Raspberry Pi Pico W
set(PICO_BOARD pico_w)

pico_sdk_init()

add_executable(PICO_MQTT main.c)

pico_enable_stdio_usb(PICO_MQTT 1)
pico_enable_stdio_uart(PICO_MQTT 0)


target_include_directories(PICO_MQTT PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)

        
target_link_libraries(PICO_MQTT
        tinyusb_board
	hardware_pwm
        pico_cyw43_arch_lwip_threadsafe_background
        pico_stdlib
        pico_lwip_mqtt
        
    )

