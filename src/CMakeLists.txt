set(target_name "RGB_LED_TURN_SIGNAL")
add_executable(${target_name})

include_directories(${CMAKE_CURRENT_LIST_DIR}/../header)
# generate the header file into the source tree as it is included in the RP2040 datasheet
pico_generate_pio_header(${target_name} ${CMAKE_CURRENT_LIST_DIR}/ws2812.pio OUTPUT_DIR ${CMAKE_CURRENT_LIST_DIR}/../header)

pico_enable_stdio_usb(${target_name} 1)
pico_enable_stdio_uart(${target_name} 1)

target_sources(${target_name} PRIVATE
    main.cpp
    seq_turn_signal.cpp
    seq_turn_signal_common.cpp
    seq_turn_signal_pattern.cpp
    )

target_link_libraries(${target_name} PRIVATE
    pico_stdlib 
    hardware_pio
    )

pico_add_extra_outputs(${target_name})