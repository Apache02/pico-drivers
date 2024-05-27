add_library(drivers_ws2812 INTERFACE)

target_sources(
        drivers_ws2812 INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/PioData.cpp
        ${CMAKE_CURRENT_LIST_DIR}/Single.cpp
        ${CMAKE_CURRENT_LIST_DIR}/Circle12.cpp
)

target_include_directories(
        drivers_ws2812 INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(
        drivers_ws2812 INTERFACE
        pico_stdlib
        hardware_pio
)
