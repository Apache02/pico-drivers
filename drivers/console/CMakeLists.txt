add_library(drivers_console INTERFACE)

target_sources(
        drivers_console INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/Console.cpp
)

target_include_directories(
        drivers_console INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(
        drivers_console INTERFACE
        pico_stdlib
)
