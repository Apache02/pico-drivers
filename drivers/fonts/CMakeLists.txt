add_library(drivers_fonts INTERFACE)

target_sources(
        drivers_fonts INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/fonts.c
)

target_include_directories(
        drivers_fonts INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/include
)
