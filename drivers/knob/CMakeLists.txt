add_library(drivers_knob INTERFACE)

target_sources(
        drivers_knob INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/ec11.cpp
)

target_include_directories(
        drivers_knob INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(
        drivers_knob INTERFACE
        pico_stdlib
)
