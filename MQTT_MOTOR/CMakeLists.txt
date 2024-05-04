cmake_minimum_required(VERSION 3.12)

include(pico_sdk_import.cmake)

project(MQTT_CONTROL C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

#Caso seja a Raspberry Pi Pico W
set(PICO_BOARD pico_w)

pico_sdk_init()

add_executable(MQTT_CONTROL main.c)

pico_enable_stdio_usb(MQTT_CONTROL 1)
pico_enable_stdio_uart(MQTT_CONTROL 0)

target_include_directories(MQTT_CONTROL PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)
target_link_libraries(MQTT_CONTROL
        tinyusb_board
	hardware_pwm
        pico_cyw43_arch_lwip_threadsafe_background
        pico_stdlib
        pico_lwip_mqtt
    )
pico_add_extra_outputs(MQTT_CONTROL)
